// Leds.h file for CE13 lab6
// By: Kyle O'Rourke
// Date: 

#ifndef LEDS_H    /* Guard against multiple inclusion */
#define LEDS_H

// Including 
#include "BOARD.h"
#include <xc.h>

/*
 *  Macro that sets the TRISE and LATE registers
 *  to 0. See the section here on macros and implement this in do-while
 *  form. Don't add a semicolon at the end of this macro!
 */
#define LEDS_INIT() do { \
TRISE = 0x00;  \
LATE = 0x00;   \
} while(0)
// Turn on the LEDs LD1 and LD4


/*
 * Macro that sets the LATE register to x. Be
 * sure to surround x in parenthesis in the macro. Also, don't add a
 * semicolon within the macro!
 */
#define LEDS_SET(x) { \
LATE = x;             \
}


/*
 * Macro that returns the value of the LATE
 * register. Don't add a semicolon within the macro!
 */
#define LEDS_GET() LATE   



#endif  // END OF Leds.h