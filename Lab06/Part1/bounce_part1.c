// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****

struct TimerResult {
    uint8_t event;
    uint8_t value;
};
struct TimerResult TResult;

#define LEFT 1
#define RIGHT 0

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
    LEDS_INIT(); // Set LEDs

    uint8_t led_value = 0x02;
    LEDS_SET(led_value);

    int direction = LEFT; // Set starting direction.

    while (1) {
        if (TResult.event == 1) {
            if (led_value == 0x01 || led_value == 0x80) {
                // Change direction
                if (led_value == 0x01) {
                    direction = LEFT;
                } else if (led_value == 0x80) {
                    direction = RIGHT;
                }
            }
            //else{
            // MOVE RIGHT:
            if (direction == RIGHT) {
                led_value = led_value >> 1;
            }
            // MOVE LEFT:
            if (direction == LEFT) {
                led_value = led_value << 1;
            }
            LEDS_SET(led_value);
            TResult.event = 0;
            //}
        }

    }
    /*
    while true:
        if the timer event flag is set:
            if we?re at the last LED:
                reverse direction
                trigger next LED
            else:
            trigger next LED
            clear the timer event flag
     */

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{

    TResult.value++;
    if (TResult.value > SWITCH_STATES()) {
        TResult.event = 1;
        TResult.value = 0;
    }
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);

}