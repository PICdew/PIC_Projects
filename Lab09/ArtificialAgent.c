// ArtificialAgent.c
// Kyle O'Rourke & Isaiah Barron
// 3/9/2018

#include "Agent.h"
#include "Protocol.h"
#include "Field.h"
#include "BOARD.h"
#include "Uart1.h"
#include "FieldOled.h"
#include "Oled.h"

// 
#include <stdlib.h>
#include <string.h>


// **** Set any macros or preprocessor directives here ****
#define HEX_ZERO 0x00
#define max32uint 4294967295

// **** Declare any data types here ****
static Field myField;
static Field enemyField;
static AgentState state;
static NegotiationData negDataRx; //theirData
static NegotiationData negDataTx; //ourData
static ProtocolParserStatus decodeReturn;
static TurnOrder whoFirs;
static GuessData guess;

typedef struct {
    int row;
    int col;
} rndCoor;

// **** Define any module-level, global, or external variables here ****
static FieldOledTurn whoTurn = FIELD_OLED_TURN_NONE;
static uint8_t validateNegData;

// **** Declare any function prototypes here ****
static void addBoatRandom(Field *myField, BoatType type);
static void GenerateRndCoor(rndCoor *coo);

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer) 
{
    if (in != '\0')
    {
        //receive a message from opponent and decode it
        decodeReturn = ProtocolDecode(in, &negDataRx, &guess);
        
    }
    
    switch (state)
    {
        case AGENT_STATE_GENERATE_NEG_DATA:
        {
            
            state = AGENT_STATE_SEND_CHALLENGE_DATA;

            // Generate our negotiation data:
            ProtocolGenerateNegotiationData(&negDataTx);
            
            //send cha message
            return ProtocolEncodeChaMessage(outBuffer, &negDataTx);
        }
        case AGENT_STATE_SEND_CHALLENGE_DATA:
        {
            //received cha message from opponent
            if (decodeReturn == PROTOCOL_PARSED_CHA_MESSAGE)
            {
                
                //record opponent data
                state = AGENT_STATE_DETERMINE_TURN_ORDER;
                        
                //send det message
                return ProtocolEncodeDetMessage(outBuffer, &negDataTx);
            }
            else if (decodeReturn == PROTOCOL_PARSING_FAILURE)
            {
                state = AGENT_STATE_INVALID;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_PARSING);
                OledUpdate();
                return FALSE;
            }
            return FALSE;
        }
        case AGENT_STATE_DETERMINE_TURN_ORDER:
        {
                
            if (decodeReturn == PROTOCOL_PARSED_DET_MESSAGE)
            {               
                
                // Validate opponent data
                validateNegData = ProtocolValidateNegotiationData(&negDataRx);

                // Check to see who goes first
                whoFirs = ProtocolGetTurnOrder(&negDataTx, &negDataRx);

                if (validateNegData == FALSE)
                {
                    //validation was incorrect
                    state = AGENT_STATE_INVALID;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(AGENT_ERROR_STRING_NEG_DATA);
                    OledUpdate();
                    return FALSE;
                } 
                else if (whoFirs == TURN_ORDER_START)
                {
                    //we win
                    state = AGENT_STATE_SEND_GUESS;
                    whoTurn = FIELD_OLED_TURN_MINE;
                    FieldOledDrawScreen(&myField, &enemyField, whoTurn);
                    return FALSE;
                } 
                else if (whoFirs == TURN_ORDER_DEFER)
                {
                    //we lose
                    state = AGENT_STATE_WAIT_FOR_GUESS;
                    whoTurn = FIELD_OLED_TURN_THEIRS;
                    FieldOledDrawScreen(&myField, &enemyField, whoTurn);
                    return FALSE;
                } 
                else if (whoFirs == TURN_ORDER_TIE)
                {
                    //Tie
                    state = AGENT_STATE_INVALID;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(AGENT_ERROR_STRING_ORDERING);
                    OledUpdate();
                    return FALSE;
                }
            }
            else if (decodeReturn == PROTOCOL_PARSING_FAILURE) // Parsing had bad value.
            {
                state = AGENT_STATE_INVALID;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_PARSING);
                OledUpdate();
                return FALSE;
            }
            return FALSE;
        }
        case AGENT_STATE_SEND_GUESS:
        {
            if (decodeReturn == PROTOCOL_PARSING_FAILURE)
            {
                state = AGENT_STATE_INVALID;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_PARSING);
                OledUpdate();
                return FALSE;
            }

            // Change state:
            state = AGENT_STATE_WAIT_FOR_HIT;
            
            // Generate random fire coordinates:
            rndCoor coo;
            GenerateRndCoor(&coo);

            // Pause for one second:
            int speed = BOARD_GetPBClock() / 8;
            int stop;
            for (stop = 0; stop <= speed; stop++) {
                //do nothing
            }
            
            // Send message:
            return ProtocolEncodeCooMessage(outBuffer, &guess);

        }
        case AGENT_STATE_WAIT_FOR_GUESS:
        {
            if ((decodeReturn == PROTOCOL_PARSED_COO_MESSAGE) && AgentGetStatus())
            {
                // Return fire! Set state to send guess.
                state = AGENT_STATE_SEND_GUESS;

                // opponent just guess, change turn to ours
                whoTurn = FIELD_OLED_TURN_MINE;

                // update my field 
                FieldRegisterEnemyAttack(&myField, &guess);

                // update screen
                FieldOledDrawScreen(&myField, &enemyField, whoTurn);

                // send hit msg
                return ProtocolEncodeHitMessage(outBuffer, &guess);
            } 
            else if ((decodeReturn == PROTOCOL_PARSED_COO_MESSAGE) && !AgentGetStatus())
            {
                state = AGENT_STATE_LOST; // :[

                //opponent guesses and won the game, change turn to none
                whoTurn = FIELD_OLED_TURN_NONE;
                FieldOledDrawScreen(&myField, &enemyField, whoTurn);
                return ProtocolEncodeHitMessage(outBuffer, &guess);
            } 
            else if (decodeReturn == PROTOCOL_PARSING_FAILURE)
            {
                state = AGENT_STATE_INVALID;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_PARSING);
                OledUpdate();
                return FALSE;
            }
            return FALSE;
        }
        case AGENT_STATE_WAIT_FOR_HIT:
        {
            if((decodeReturn == PROTOCOL_PARSED_HIT_MESSAGE) && AgentGetEnemyStatus())
            {
                state = AGENT_STATE_WAIT_FOR_GUESS;
                
                //we hit a boat, now update enemy field
                FieldUpdateKnowledge(&enemyField, &guess);
                
                //now since we got a hit, its thier turn to gues
                whoTurn = FIELD_OLED_TURN_THEIRS;
                FieldOledDrawScreen(&myField, &enemyField, whoTurn);
                return FALSE;
            }
            else if((decodeReturn == PROTOCOL_PARSED_HIT_MESSAGE) && !AgentGetEnemyStatus())
            {
                state = AGENT_STATE_WON;
                
                //game over we lost
                whoTurn = FIELD_OLED_TURN_NONE;
                FieldOledDrawScreen(&myField, &enemyField, whoTurn);
                return FALSE;
            }
            else if (decodeReturn == PROTOCOL_PARSING_FAILURE)
            {
                state = AGENT_STATE_INVALID;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_PARSING);
                OledUpdate();
                return FALSE;
            }
            return FALSE;
        }
        case AGENT_STATE_INVALID: {
            break;
        }
        case AGENT_STATE_LOST: {
            break;
        }
        case AGENT_STATE_WON: {
            break;
        }
    }
    return FALSE;
}

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 *   Implement an AgentInit() function that randomly places the boats. There
 *   should be no pattern or hard-coded layout, it should be entirely random on
 *   a ship-by-ship basis. This also applies to their direction, so that all possible
 *   ship layouts could be generated by your code.
 */
void AgentInit(void)
{
    state = AGENT_STATE_GENERATE_NEG_DATA;

    // Step 0: Clear field and fill with empty space.
    FieldInit(&myField, FIELD_POSITION_EMPTY);

    //Step 1: set enemy field to unknown
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);

    // Step 2: Add huge boat to field:
    addBoatRandom(&myField, FIELD_BOAT_HUGE);

    // Step 3: Add large boat to field:
    addBoatRandom(&myField, FIELD_BOAT_LARGE);

    // Step 4: Add medium boat to field:
    addBoatRandom(&myField, FIELD_BOAT_MEDIUM);

    // Step 5: Add small boat to field:
    addBoatRandom(&myField, FIELD_BOAT_SMALL);

    // Step 6: Draw playing field:
    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
}


/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void)
{
    return (FieldGetBoatStates(&myField));
}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void)
{
    return (FieldGetBoatStates(&enemyField));

}


//helper Function that generates a random coordinate for AgentRun

static void GenerateRndCoor(rndCoor * coo) 
{
    while (TRUE)
    {
        coo->row = rand() % FIELD_ROWS;
        coo->col = rand() % FIELD_COLS;

        FieldPosition pos;

        pos = FieldAt(&enemyField, coo->row, coo->col);

        if (pos == FIELD_POSITION_UNKNOWN) {
            guess.row = coo->row;
            guess.col = coo->col;
            return;
        }
    }
}

// Function that adds a boat to a random & valid location.

static void addBoatRandom(Field *myField, BoatType type)
{

    while (TRUE)
    {

        // Step 1: Pick random location for Col and Row
        uint8_t row = (rand()) % ((FIELD_ROWS - 1) + 1);
        uint8_t col = (rand()) % ((FIELD_COLS - 1) + 1);

        // Step 2: Pick random direction:
        BoatDirection dir = (rand()) % (3 + 1);

        // Step 3: Verify if given parameters are acceptable.
        if (FieldAddBoat(myField, row, col, dir, type)) {
            // Success
            return;
        } else {
            // Failure. Try again.
            continue;
        }

    } // While end

}
