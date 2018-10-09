// Heap size: 2048

// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Morse.h"
#include "Buttons.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static char top_CharArray[OLED_CHARS_PER_LINE]; // Shows all the dots and dashes.
static char bot_CharArray[OLED_CHARS_PER_LINE]; // Shows the decoded message in chars. (+1 for \n)
static int topCharCount = 0;
static int botCharCount = 0;
static uint8_t timerFlag;

static char dashC = '-';
static char dotC = '.';

// **** Declare any function prototypes here ****
void clearOLED_top();
int addTopCharOLED(char C);
int addBotCharOLED(char C);

int main()
{
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    // MAIN START:
    OledInit(); // Start the OLED.
    if (!MorseInit()) { // Create the tree.
        printf("MorseInit Failed!\n");
        FATAL_ERROR();
    }


    // ************* TEST CODE START *************
    //    // Run through to V:
    //    MorseDecode(MORSE_CHAR_DOT); // E
    //    addTopCharOLED(dotC);
    //    MorseDecode(MORSE_CHAR_DOT); // I
    //    addTopCharOLED(dotC);
    //    MorseDecode(MORSE_CHAR_DASH); // V
    //    addTopCharOLED(dashC);
    //    MorseDecode(MORSE_CHAR_DOT); // F
    //    addTopCharOLED(dotC);
    //    char output;
    //    output = MorseDecode(MORSE_CHAR_END_OF_CHAR); // RETURN F
    //    addBotCharOLED(output); 
    //    MorseDecode(MORSE_CHAR_DECODE_RESET); // RESET
    //    int i;
    //    for(i = 0; i < 19999999; i++){}
    //    clearOLED_top();
    //    MorseDecode(MORSE_CHAR_DASH); // 
    //    addTopCharOLED(dashC);
    //    MorseDecode(MORSE_CHAR_DASH); // 
    //    addTopCharOLED(dashC);
    //    MorseDecode(MORSE_CHAR_DASH); // 
    //    addTopCharOLED(dashC);
    //    output = MorseDecode(MORSE_CHAR_END_OF_CHAR); // RETURN O
    //    addBotCharOLED(output);
    //    MorseDecode(MORSE_CHAR_DECODE_RESET); // RESET
    // ************* TEST CODE END *************


    MorseEvent events = MORSE_EVENT_NONE;
    char decodeReturn;
    while (1) {
        if (timerFlag) {
            events = MorseCheckEvents();
            timerFlag = FALSE;
        } else {
            events = MORSE_EVENT_NONE;
        }
        switch (events) {
        case MORSE_EVENT_NONE:
        {
            // Do nothing.
            break;
        }
        case MORSE_EVENT_DOT:
        {
            // Send a dot to the decode function.
            decodeReturn = MorseDecode(MORSE_CHAR_DOT);
            addTopCharOLED(dotC);
            break;
        }
        case MORSE_EVENT_DASH:
        {
            // Send a dash to the decode function.
            decodeReturn = MorseDecode(MORSE_CHAR_DASH);
            addTopCharOLED(dashC);
            break;
        }
        case MORSE_EVENT_INTER_LETTER:
        {
            // Output current letter move on to next letter. Reset the tree traverse.
            decodeReturn = MorseDecode(MORSE_CHAR_END_OF_CHAR); // Return current char value.
            if (decodeReturn == NULL || decodeReturn == '#') {
                decodeReturn = '#';
            }
            addBotCharOLED(decodeReturn); // Print new char to bottom.
            MorseDecode(MORSE_CHAR_DECODE_RESET); // Reset to top of tree.
            clearOLED_top(); // Clear the top oled.
            break;
        }
        case MORSE_EVENT_INTER_WORD:
        {
            // Move on to new word. Output a space and reset the tree.
            decodeReturn = MorseDecode(MORSE_CHAR_END_OF_CHAR);
            if (decodeReturn == NULL || decodeReturn == '#') {
                decodeReturn = '#';
            }
            MorseDecode(MORSE_CHAR_DECODE_RESET); // Reset to top of tree.
            clearOLED_top(); // Clear the top oled.
            addBotCharOLED(' ');
            addBotCharOLED(decodeReturn);
            break;
        }
        }
    } // End while(1);




    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/
    while (1);
}

// Function that clears the top line of the OLED and updates it with the new current array.

void clearOLED_top()
{
    // CLEAR PREVIOUS top_Char values:
    int i;
    for (i = 0; i <= OLED_CHARS_PER_LINE - 1; i++) {
        top_CharArray[i] = ' ';
    }
    topCharCount = 0;
    // Print the new values:
    OledDrawString(top_CharArray);
    OledUpdate();
}

// Function that adds character to top line and updates the OLED.

int addTopCharOLED(char C)
{
    // Max chars per line = 21.
    // Since the array is 0 indexed then we need to max at 21-1.
    if (topCharCount >= (OLED_CHARS_PER_LINE - 1)) {
        // Return an error: MAX Size.
        return STANDARD_ERROR;
    } else {
        top_CharArray[topCharCount] = C;
        OledDrawString(top_CharArray); // Add change to the queue.
        OledUpdate(); // Push the change to the screen.
        topCharCount++; // Increment the top location.
        return 1;
    }
    return 1;
}

// Function that adds character to bottom line and updates the OLED.

int addBotCharOLED(char C)
{
    //bot_CharArray[0] = '\n'; // Make sure that the first is always a newline.
    char ptOLED[200];
    // Max chars per line = 21. (New line at [0] does not count so add 1.)
    // bot_CharArray is a 22 sized array.
    // Since the array is 0 indexed then we need to max at 21.
    //if (botCharCount >= (OLED_CHARS_PER_LINE - 1)) {
    //    // Return an error: MAX Size.
    //    return STANDARD_ERROR;
    //} else {
        bot_CharArray[botCharCount] = C;
        sprintf(ptOLED, "\n%s", bot_CharArray);
        OledDrawString(ptOLED); // Add change to the queue.
        OledUpdate(); // Push the change to the screen.
        botCharCount++; // Increment the top location.
        return 1;
    //}
    //return 1;
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    timerFlag = TRUE;
}
