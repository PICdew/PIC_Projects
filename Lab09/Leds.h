/* 
 * File:   Leds.h
 * Author: Isaiah Barron
 *
 * Created on February 17, 2018, 3:17 PM
 */

#ifndef LEDS_H
#define LEDS_H

#include "BOARD.h"
#include <xc.h>

//the LATE controls the E-pins of the LEDs and the TRISE turns E-pins into inputs or outputs
#define LEDS_INIT() do{ \
TRISE = 0x00; \
LATE = 0x00;  \
}while(0)

#define LEDS_GET() LATE //returns the value of the LATE register

#define LEDS_SET(x) LATE = (x) //changes the value of the LATE register

#endif