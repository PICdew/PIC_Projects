// Protocol.c file
// Kyle O'Rourke & Isaiah Barron
// 3/9/2018

//Libraries
#include "Protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "BOARD.h"
#include "Oled.h"

// **** Set any macros or preprocessor directives here ****
#define MAX_16UINT 65535
#define VALID_HEX(x) ((x) <= 15)
#define TURN_ORDER_WRONG -2


// **** Declare any data types here ****
//Parsing Algorithm enum for statemachine

typedef enum {
    WAITING, RECORDING, FIRST_CHECKSUM_HALF, SECOND_CHECKSUM_HALF, NEWLINE
} AlgorithmState;
//data struct: sentence holds the input data from the protocol encoded messages
//and checksum is to store the checksum value from the protocol encoded messages

typedef struct {
    char sentence[PROTOCOL_MAX_MESSAGE_LEN];
    uint8_t checkSum;
    int index;
    AlgorithmState state;
} AlgorithmData;

// **** Define any module-level, global, or external variables here ****
AlgorithmData parsing = {.state = WAITING};

// **** Declare any function prototypes here ****
static uint8_t checkSum(char *message);
static uint8_t HexToInt(char in);

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * @param message The character array used for storing the output. Must be long enough to store the
 *                entire string, see PROTOCOL_MAX_MESSAGE_LEN.
 * @param data The data struct that holds the data to be encoded into `message`.
 * @return The length of the string stored into `message`.
 */
int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    int length; //used to return the length of string stored in message
    char output[PROTOCOL_MAX_MESSAGE_LEN]; //output stores the coordinate data

    sprintf(output, PAYLOAD_TEMPLATE_COO, data->row, data->col);
    sprintf(message, MESSAGE_TEMPLATE, output, checkSum(output)); //this wraps message into a NMEA0183 protocol message

    length = strlen(message);
    return length;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    //Code follows from ProtocolEncodeCooMessage, only thing that changes is what data is being
    //stored in output

    int length;
    char output[PROTOCOL_MAX_MESSAGE_LEN];

    sprintf(output, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);
    sprintf(message, MESSAGE_TEMPLATE, output, checkSum(output));

    length = strlen(message);
    return length;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    //Code follows from ProtocolEncodeCooMessage, only thing that changes is what data is being
    //stored in output

    int length;
    char output[PROTOCOL_MAX_MESSAGE_LEN];

    sprintf(output, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);
    sprintf(message, MESSAGE_TEMPLATE, output, checkSum(output));

    length = strlen(message);
    return length;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    //Code follows from ProtocolEncodeCooMessage, only thing that changes is what data is being
    //stored in output	

    int length;
    char output[PROTOCOL_MAX_MESSAGE_LEN];

    sprintf(output, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);
    sprintf(message, MESSAGE_TEMPLATE, output, checkSum(output));

    length = strlen(message);
    return length;
}

/**
 * This function decodes a message into either the NegotiationData or GuessData structs depending
 * on what the type of message is. This function receives the message one byte at a time, where the
 * messages are in the format defined by MESSAGE_TEMPLATE, with payloads of the format defined by
 * the PAYLOAD_TEMPLATE_* macros. It returns the type of message that was decoded and also places
 * the decoded data into either the `nData` or `gData` structs depending on what the message held.
 * The onus is on the calling function to make sure the appropriate structs are available (blame the
 * lack of function overloading in C for this ugliness).
 *
 * PROTOCOL_PARSING_FAILURE is returned if there was an error of any kind (though this excludes
 * checking for NULL pointers), while
 * 
 * @param in The next character in the NMEA0183 message to be decoded.
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */

ProtocolParserStatus ProtocolDecode(const char in, NegotiationData *nData, GuessData *gData)
{
    uint8_t intChar = HexToInt(in);

    switch (parsing.state) {
        case WAITING:
        {
            if (in == '$') {
                parsing.state = RECORDING;
                parsing.index = 0;
                return PROTOCOL_PARSING_GOOD;
            }
            else
            {
                parsing.state = WAITING;
                return PROTOCOL_WAITING;
            }
            break;
        }
        case RECORDING:
        {
            if (in != '*')
            {
                //if not a *, then put the data "in" in an array 
                //and increment the index. 
                //this is how we parse the data from each protocol message and the message id
                parsing.state = RECORDING;
                parsing.sentence[parsing.index] = in;
                parsing.index += 1;
                return PROTOCOL_PARSING_GOOD;
            } 
            else if (in == '*')
            {
                parsing.state = FIRST_CHECKSUM_HALF;
                return PROTOCOL_PARSING_GOOD;
            }
            break;
        }
        case FIRST_CHECKSUM_HALF:
        {
            //IM NOT IF THIS IF STATEMENT IS CORRECT
            if (VALID_HEX(intChar))
            {
                parsing.state = SECOND_CHECKSUM_HALF;
                //to save the top 4 bits of the hex value, shift over to left by 4 after the conversion
                parsing.checkSum = (HexToInt(in) << 4);
                return PROTOCOL_PARSING_GOOD;
            } 
            else
            {
                parsing.state = WAITING;
                return PROTOCOL_PARSING_FAILURE;
            }
            break;
        }
        case SECOND_CHECKSUM_HALF:
        {

            parsing.checkSum |= HexToInt(in);

            // not valid hex or checksums dont match
            if (!VALID_HEX(intChar) || (checkSum(parsing.sentence) != parsing.checkSum))
            {
                // Add the value to the current checksum:
                parsing.state = WAITING;
                return PROTOCOL_PARSING_FAILURE;
            }                //successful hex and checksum
            else if (VALID_HEX(intChar) && (checkSum(parsing.sentence) == parsing.checkSum))
            {
                // Success checksums are equal and hex is valid.
                parsing.state = NEWLINE;
                parsing.sentence[parsing.index] = '\0';
                return PROTOCOL_PARSING_GOOD;
            }
            break;
        }
        case NEWLINE:
        {
            int i; //for loop iteration
            char *MSGID;
            MSGID = strtok(parsing.sentence, ","); //to tokenize the message id 'HIT', 'COO', 'CHA' or 'DET'

            if ((in != '\n') || ((strcmp("COO", MSGID) != 0) && (strcmp("HIT", MSGID) != 0) && (strcmp("CHA", MSGID) != 0)
                    && (strcmp("DET", MSGID) != 0)))
            {
                parsing.state = WAITING;
                return PROTOCOL_PARSING_FAILURE;
            } 
            else
            {
                // It returns the type of message that was decoded and also places
                // the decoded data into either the `nData` or `gData` structs depending on what the message held.
                // The onus is on the calling function to make sure the appropriate structs are available

                if (strcmp("COO", MSGID) == 0) {
                    parsing.state = WAITING;
                    gData->row = HexToInt(parsing.sentence[4]);
                    gData->col = HexToInt(parsing.sentence[6]);

                    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
                        parsing.sentence[i] = '\0';
                    }

                    return PROTOCOL_PARSED_COO_MESSAGE;
                }
                else if (strcmp("HIT", MSGID) == 0)
                {
                    parsing.state = WAITING;
                    gData->row = HexToInt(parsing.sentence[4]);
                    gData->col = HexToInt(parsing.sentence[6]);
                    gData->hit = HexToInt(parsing.sentence[8]);

                    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
                        parsing.sentence[i] = '\0';
                    }

                    return PROTOCOL_PARSED_HIT_MESSAGE;
                } 
                else if (strcmp("CHA", MSGID) == 0)
                {
                    parsing.state = WAITING;

                    //to save data that was parsed
                    char *encryVal;
                    char *hashVal;

                    // Get the encryptedGuess data:
                    encryVal = strtok(NULL, ",");
                    nData->encryptedGuess = atoi(encryVal);

                    // Get hash:
                    hashVal = strtok(NULL, "*"); 
                    nData->hash = atoi(hashVal);

                    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
                        parsing.sentence[i] = '\0';
                    }

                    return PROTOCOL_PARSED_CHA_MESSAGE;
                } 
                else if (strcmp("DET", MSGID) == 0)
                {
                    
                    
                    parsing.state = WAITING;

                    //to save data that was parsed
                    char *keyVal;
                    char *guessVal;

                    // Get the guess data:
                    guessVal = strtok(NULL, ","); 
                    nData->guess = atoi(guessVal);
                    

                    // Get encryptionKey:
                    keyVal = strtok(NULL, "*"); 
                    nData->encryptionKey = atoi(keyVal);

                    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
                        parsing.sentence[i] = '\0';
                    }

                    return PROTOCOL_PARSED_DET_MESSAGE;
                }
            }
            break;
        }
    }
    return PROTOCOL_PARSING_FAILURE;
}

/**
 * This function generates all of the data necessary for the negotiation process used to determine
 * the player that goes first. It relies on the pseudo-random functionality built into the standard
 * library. The output is stored in the passed NegotiationData struct. The negotiation data is
 * generated by creating two random 16-bit numbers, one for the actual guess and another for an
 * encryptionKey used for encrypting the data. The 'encryptedGuess' is generated with an
 * XOR(guess, encryptionKey). The hash is simply an 8-bit value that is the XOR() of all of the
 * bytes making up both the guess and the encryptionKey. There is no checking for NULL pointers
 * within this function.
 * @param data The struct used for both input and output of negotiation data.
 */
void ProtocolGenerateNegotiationData(NegotiationData *data)
{

    // Basically: Make it manipulate the given data array to...
    // Use the rand() library functions.
    // M + (int)(rand() * (N-M+1))  from M to N.
    // Since max value of 16bit is 65535 M = 65535.

    data->guess = (uint16_t) (rand() & 0x0000FFFF); // A random 16 bit number.
    data->encryptionKey = (uint16_t) (rand() & 0x0000FFFF); // A random 16 bit number.
    data->encryptedGuess = data->guess ^ data->encryptionKey; // XOR(guess, encryptionKey)

    //data->hash = (uint8_t)(data->guess ^ data->encryptionKey); // The hash is simply an 8-bit value that is the XOR() of all of the
    // bytes making up both the guess and the encryptionKey.
    // The above was incorrect. (I think...) New method below:
    // "Agent A transmits a checksum of both its guess and key (which is an 8-bit
    // XOR of all of their bytes) along with an encrypted version of its guess (which is a
    // 16-bit XOR of the guess with the encryptionKey)."
    // So grab the top 8-bits of both values and xor them together. Then get bottom 8-bits and
    // XOR them together. XOR the result. (Or just xor them and then xor the result with itself.)
    
    data->hash = data->guess ^ data->encryptionKey; // XOR guess and encrypt values.
    uint16_t upper = (uint8_t) (data->hash >> 8); // Grab upper 8-bits.
    uint16_t lower = (uint8_t) (data->hash & 0x00ff); // Grab lower 8-bits.
    data->hash = upper ^ lower; // Now hash is the XOR of the previous value's mid.

}

/**
 * Validates that the negotiation data within 'data' is correct according to the algorithm given in
 * GenerateNegotitateData(). Used for verifying another agent's supplied negotiation data. There is
 * no checking for NULL pointers within this function. Returns TRUE if the NegotiationData struct
 * is valid or FALSE on failure.
 * @param data A filled NegotiationData struct that will be validated.
 * @return TRUE if the NegotiationData struct is consistent and FALSE otherwise.
 */
uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    // Check the hash and encryptedGuess data:
    // We basically need to 'undo' the XOR stuff and verify the values.

    // Check the encryptedGuess value:
    if (data->encryptedGuess != (data->guess ^ data->encryptionKey))
    {
        return FALSE; // Incorrect!
    }

    // Check the hash value:
    uint16_t upper = (uint8_t) (data->hash >> 8); // Grab upper 8-bits.
    uint16_t lower = (uint8_t) (data->hash & 0x00ff); // Grab lower 8-bits.
    uint16_t hashCheck = (upper ^ lower); // Now hash is the XOR of the previous value's mid.

    if (data->hash != hashCheck)
    {
        return FALSE; // Incorrect!
    }

    return TRUE;
}

/**
 * This function returns a TurnOrder enum type representing which agent has won precedence for going
 * first. The value returned relates to the agent whose data is in the 'myData' variable. The turn
 * ordering algorithm relies on the XOR() of the 'encryptionKey' used by both agents. The least-
 * significant bit of XOR(myData.encryptionKey, oppData.encryptionKey) is checked so that if it's a
 * 1 the player with the largest 'guess' goes first otherwise if it's a 0, the agent with the
 * smallest 'guess' goes first. The return value of TURN_ORDER_START indicates that 'myData' won,
 * TURN_ORDER_DEFER indicates that 'oppData' won, otherwise a tie is indicated with TURN_ORDER_TIE.
 * There is no checking for NULL pointers within this function.
 * @param myData The negotiation data representing the current agent.
 * @param oppData The negotiation data representing the opposing agent.
 * @return A value from the TurnOrdering enum representing which agent should go first.
 */
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{
    // Simply put:
    // We have two structs one (myData) is used for incoming values and the other (oppData)
    // is used for opponent data.

    // Step 1: XOR encryptKey of both players:
    uint16_t encryptKeyXOR = (myData->encryptionKey) ^ (oppData->encryptionKey);

    //Step 2: Check if LSB(Least Significant Bit) of encryptKeyXOR is either 1 or 0:
    uint16_t keyLSB = encryptKeyXOR & 0x1;
    
    if (keyLSB == 1)
    { 
        // It is one. Player with the larger 'guess' goes first!
        if (myData->guess == oppData->guess) 
        { // TIE!
            return TURN_ORDER_TIE;
        } 
        else if (myData->guess > oppData->guess)
        { // myData goes first!
            return TURN_ORDER_START;
        } 
        else if (myData->guess < oppData->guess)
        { // oppData goes first!
            return TURN_ORDER_DEFER;
        }
    } 
    else if (keyLSB == 0)
    { 
        // It is zero.
        if (myData->guess == oppData->guess)
        { // TIE!
            return TURN_ORDER_TIE;
        } 
        else if (myData->guess < oppData->guess)
        { // myData goes first!
            return TURN_ORDER_START;
        } 
        else if (myData->guess > oppData->guess)
        { // oppData goes first!
            return TURN_ORDER_DEFER;
        }
    }
    return TURN_ORDER_WRONG; // SOMETHING WENT HORRIBLY WRONG!
}


// Checksum function for the NMEA 0183 protocol.
// Uses XOR for all values.

static uint8_t checkSum(char *message) {
    // Checksum
    uint8_t checkSum = 0;
    while (*message) {
        checkSum ^= *message++;
    }
    return checkSum;
}

//ASCII hex conversion to 8 bit integer
//returns an 8 bit integer

static uint8_t HexToInt(char in) {
    uint8_t returnVal = 0; // Value to return.

    //hex values 0 - 9
    if (in >= 48 && in <= 57) {
        returnVal = in - 48;
    }        //hex values a - f
    else if (in >= 97 && in <= 102) {
        returnVal = in - 87;
    }        //hex values A - F
    else if (in >= 65 && in <= 70) {
        returnVal = in - 55;
    }
    return returnVal;
}




