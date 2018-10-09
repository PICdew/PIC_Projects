// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****

struct ButtonEvent {
    uint8_t event;
    uint8_t value;
};
struct ButtonEvent ButtonEvent;

#define TRUE 1
#define FALSE 0


// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // BTN1 - Toggles LD1 and LD2
    // BTN2 - Toggles LD3 and LD4
    // BTN3 - Toggles LD5 and LD6
    // BTN4 - Toggles LD7 and LD8
    // The behavior of the button presses to the LEDs will change based on the switch locations.

    LEDS_INIT(); // Set the output LED pins.
    ButtonsInit(); // Set the button inputs

    uint8_t led_out_val = 0x00;
    LEDS_SET(led_out_val);

    // Main while loop for program code:
    /*
    while (TRUE) {
        if (ButtonEvent.event) {
            //ButtonEvent.value = Current button values.
            //if ()
                ButtonEvent.event = FALSE;
        }
    }
     */



    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Function will poll ButtonsCheckEvents here for new changes.
    uint8_t btnPreCheck = ButtonEvent.value; // Save previous value.
    ButtonEvent.value = ButtonsCheckEvents(); // Get new value.

    if (btnPreCheck != ButtonEvent.value) { // If new != old even happened.
        ButtonEvent.event = TRUE;
    }

    // Clear the interrupt flag.
    INTClearFlag(INT_T1);

}