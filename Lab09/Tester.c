// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Buttons.h"
//#include "Oled.h"
#include "Field.h"
#include "Protocol.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static uint32_t counter;
static uint8_t buttonEvents;
Field myField;
GuessData gData;
NegotiationData nData;

// **** Declare any function prototypes here ****



int main()
{
    BOARD_Init();

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a 10ms timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Disable buffering on stdout
    setbuf(stdout, NULL);

    ButtonsInit();

//    OledInit();

    // Prompt the user to start the game and block until the first character press.
//    OledDrawString("Press BTN4 to start.");
//    OledUpdate();
    //while ((buttonEvents & BUTTON_EVENT_4UP) == 0);

/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
    
    ///////////////////TEST for Field.c
    /*FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldAddBoat(&myField, 2, 3, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
    FieldAddBoat(&myField, 3, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
    FieldAddBoat(&myField, 4, 4, FIELD_BOAT_DIRECTION_WEST, FIELD_BOAT_MEDIUM);
    FieldAddBoat(&myField, 5, 3, FIELD_BOAT_DIRECTION_WEST, FIELD_BOAT_LARGE);
    
    
    //should print position hit in field array and hit status should be hit
    //huge boat has 6 lives
    //opponent guesses row 2 col 3
    gData.row = 2;
    gData.col = 3;
    FieldRegisterEnemyAttack(&myField, &gData);
    
    //opponent guesses row 2 col 4
    gData.row = 2;
    gData.col = 4;
    FieldRegisterEnemyAttack(&myField, &gData);
    
    //opponent guesses row 2 col 5
    gData.row = 2;
    gData.col = 5;
    FieldRegisterEnemyAttack(&myField, &gData);
    
    //opponent guesses row 2 col 6
    gData.row = 2;
    gData.col = 6;
    FieldRegisterEnemyAttack(&myField, &gData);
    
    //opponent guesses row 2 col 7
    gData.row = 2;
    gData.col = 7;
    FieldRegisterEnemyAttack(&myField, &gData);
    
    //opponent guesses row 2 col 8
    gData.row = 2;
    gData.col = 8;
    FieldRegisterEnemyAttack(&myField, &gData);
    
    uint8_t status = FieldGetBoatStates(&myField);
    printf("boat status: %02x\n", status);
    
    //should output 7 for FIELD_POSITION_HIT
    printf("data in field array: %i\n", myField.field[gData.row][gData.col]);
    
    //hit status of the boat
    printf("data for hit status: %i\n", gData.hit);
    
    
    gData.row = 2;
    gData.col = 9;
    FieldRegisterEnemyAttack(&myField, &gData);
    //field position miss = 5
    printf("data in field array: %i\n", myField.field[gData.row][gData.col]);
    //hit status miss = 0
    printf("data for hit status: %i\n", gData.hit);*/
    
    /*int i, j;
    for(i = 0; i < FIELD_ROWS; i++)
    {
        for(j = 0; j < FIELD_COLS; j++)
        {
            printf("field[%i][%i]: %i\n", i, j, myField.field[i][j]);
        }
    }*/
    
    ///////////////////TEST for Protocol.c
    //protocol encode message function
    /*char testMsg[PROTOCOL_MAX_MESSAGE_LEN];
    gData.row = 0;
    gData.col = 4;
    gData.hit = HIT_HIT;
    int tempLen = ProtocolEncodeHitMessage(testMsg, &gData);
    printf("message: %s\n", testMsg);
    printf("length of message: %i\n", tempLen);*/
    
    //ProtocolDecode function
    
    /*PROTOCOL_PARSING_FAILURE = -1,   // Parsing failed for some reason. Could signify and unknown
                                      // message was received or the checksum was incorrect.
    PROTOCOL_WAITING,      //0        // Parsing is waiting for the starting '$' of a new message
    PROTOCOL_PARSING_GOOD,    //1     // A success value that indicates no message received yet.
    
    PROTOCOL_PARSED_COO_MESSAGE, //2  // Coordinate message. This is used for exchanging guesses.
    PROTOCOL_PARSED_HIT_MESSAGE, //3  // Hit message. Indicates a response to a Coordinate message.
    PROTOCOL_PARSED_CHA_MESSAGE, //4  // Challenge message. Used in the first step of negotiating the
                                      // turn order.
    PROTOCOL_PARSED_DET_MESSAGE  //5  // Determine message. Used in the second and final step of
                                      // negotiating the turn order.*/
    
    char testMsg2[PROTOCOL_MAX_MESSAGE_LEN];
    nData.encryptedGuess = 62132;
    nData.hash = 70;
    int tempLen2 = ProtocolEncodeChaMessage(testMsg2, &nData);
    printf("message: %s\n", testMsg2);
    printf("length of message: %i\n", tempLen2);
    
    
    int i;
    int p;
    char in;
    
    for(i = 0; i < tempLen2; i++)
    {
        in = testMsg2[i];
        p = ProtocolDecode(in, &nData, &gData);
        printf("Protocol return message: %i %i %c\n", p, i, in);
    }
    
    
/******************************************************************************
 * Your code goes in between this comment and the preceeding one with asterisks
 *****************************************************************************/

   while (1);
}




/**
 * This is the interrupt for the Timer2 peripheral. It just keeps incrementing a counter used to
 * track the time until the first user input.
 */

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // Increment a counter to see the srand() function.
    counter++;

    // Also check for any button events
    buttonEvents = ButtonsCheckEvents();
}