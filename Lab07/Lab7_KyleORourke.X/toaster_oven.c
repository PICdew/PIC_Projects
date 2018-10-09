// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Ascii.h"
#include "Adc.h"
#include "Leds.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>


// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

// **** Declare any datatypes here ****

// OVEN DATA HOLDING STRUCT

typedef struct {
    uint16_t cooking_time_left; // Int that is how much cook time is left.
    uint16_t initial_cook_time; // Int that is the total cooking time.
    uint16_t temperature; // Int that holds current temp value.
    int mode; // Int value that is the current cook mode.
    int oven_state; // Int value that is the current state.
    uint16_t button_press_counter; // Counter for time button pressed.
    int input_selection; // Pot effects time or temp.
} ovens_data;

static enum {
    RESET,
    START,
    PENDING_SEL_CHANGE,
    COUNTDOWN,
    PENDING_RESET
};

// **** Function declarations ****
void update_display(ovens_data data); // Updates the display from given oven_data input.
uint32_t seconds_to_min(uint16_t in); // Finds a value for number of min in given number of seconds.
uint16_t seconds_in_time(uint16_t in); // Finds a value for seconds in current min value.

// **** Define any module-level, global, or external variables here ****
static uint8_t buttonValues;
static uint16_t timerCounter; // Counter for the 5Hz timer

#define BAKE 0
#define TOAST 1
#define BROIL 2

#define TRUE 1
#define FALSE 0

#define LONG_PRESS 5

#define DEFAULT_TEMP 350
#define BROIL_TEMP 500
#define TIME_SEL 0
#define TEMP_SEL 1

#define led_0 0x00
#define led_1 0x80
#define led_2 0xC0
#define led_3 0xE0
#define led_4 0xF0
#define led_5 0xF8
#define led_6 0xFC
#define led_7 0xFE
#define led_8 0xFF

// Display variables
static char oven_ON_top[] = {'\x01', '\x01', '\x01', '\x01', '\x01'};
static char oven_OFF_top[] = {'\x02', '\x02', '\x02', '\x02', '\x02'};
static char oven_ON_bot[] = {'\x03', '\x03', '\x03', '\x03', '\x03'};
static char oven_OFF_bot[] = {'\x04', '\x04', '\x04', '\x04', '\x04'};

// Flag variables
static uint8_t twoHzFlag;

// Configuration Bit settings

int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    OledInit(); // Setting the OLED initializer.
    ButtonsInit(); // Setting the button lib. initializer.
    AdcInit(); // Sets the adc system.
    LEDS_INIT(); // Setting the LED initializations.

    static ovens_data oven_data; // Declare main struct value.
    oven_data.oven_state = RESET; // Set the initial state = RESET.

    //    // TEST CODE
    //    oven_data.mode = TOAST;
    //    oven_data.input_selection = TEMP_SEL;
    //    oven_data.oven_state = COUNTDOWN;
    //    oven_data.temperature = DEFAULT_TEMP;
    //    
    //    update_display(oven_data);
    //    
    //    // buttonValues = the button state
    //    
    //    while(TRUE){};

    // MAIN PROGRAM LOOP:
    while (TRUE) {
        // STATE MACHINE SWITCH STATEMENTS:
        switch (oven_data.oven_state) {
        case RESET:
        {
            oven_data.mode = BAKE;
            oven_data.temperature = DEFAULT_TEMP;
            oven_data.input_selection = TIME_SEL;
            oven_data.oven_state = START;
            update_display(oven_data);
            break;
        }
        case START:
        {
            // If AdcChanged update either time or temp:
            if (AdcChanged) {
                if (oven_data.input_selection == TIME_SEL) {
                    // Change initial time value. oven_data.initial_cook_time = XX
                    uint16_t newAdcVal = AdcRead();
                    // "using only the top 8 bits and adding 1"
                    // Bit shift to the right 2 times to remove bottom two bits:
                    newAdcVal = newAdcVal >> 2;
                    newAdcVal += 1;
                    oven_data.initial_cook_time = newAdcVal;

                    update_display(oven_data);
                }
                if (oven_data.input_selection == TEMP_SEL) {
                    // Change the temp value oven_data.temperature = XX
                    uint16_t newAdcVal = AdcRead();
                    // "only the top 8 bits of the ADC value and adding 300 to it"
                    // Bit shift to the right 2 times to remove bottom two bits:
                    newAdcVal = newAdcVal >> 2;
                    newAdcVal += 300;
                    oven_data.temperature = newAdcVal;

                    update_display(oven_data);
                }
            }
            // Look to see if selector change is happening:
            if (buttonValues & BUTTON_EVENT_3DOWN) {
                //timerCounter holds current 5Hz count.
                oven_data.button_press_counter = timerCounter;
                oven_data.oven_state = PENDING_SEL_CHANGE;
                break;
            }
            // Look to see if start is going:
            if (buttonValues & BUTTON_EVENT_4DOWN) {
                oven_data.oven_state = COUNTDOWN;
                oven_data.cooking_time_left = oven_data.initial_cook_time * 2; //2x for 2Hz timer use.
                twoHzFlag = FALSE;
                break;
            }
            break;
        }
        case PENDING_SEL_CHANGE:
        {
            while (oven_data.oven_state = PENDING_SEL_CHANGE) { // Need to wait in state until action is over.
                if (((timerCounter - oven_data.button_press_counter) > LONG_PRESS) && (buttonValues & BUTTON_EVENT_3UP)) {
                    // Change input selection for temp or time.
                    // Make change on button release and if sate is bake:
                    if (oven_data.mode == BAKE) {
                        switch (oven_data.input_selection) {
                        case TIME_SEL:
                            oven_data.input_selection = TEMP_SEL;
                            break;
                        case TEMP_SEL:
                            oven_data.input_selection = TIME_SEL;
                            break;
                        }
                    } else {
                        oven_data.input_selection = TIME_SEL;
                    }
                    update_display(oven_data);
                    oven_data.oven_state = START;
                    break;
                } else if (((timerCounter - oven_data.button_press_counter) < LONG_PRESS) && (buttonValues & BUTTON_EVENT_3UP)) {
                    // Change mode: Bake -> Toast -> Broil -> Bake.
                    if (buttonValues & BUTTON_EVENT_3UP) {
                        switch (oven_data.mode) {
                        case BAKE:
                            oven_data.mode = TOAST;
                            oven_data.input_selection = TIME_SEL;
                            oven_data.oven_state = START;
                            break;
                        case TOAST:
                            oven_data.mode = BROIL;
                            oven_data.input_selection = TIME_SEL;
                            oven_data.temperature = BROIL_TEMP;
                            oven_data.oven_state = START;
                            break;
                        case BROIL:
                            oven_data.mode = BAKE;
                            oven_data.input_selection = TIME_SEL;
                            oven_data.temperature = DEFAULT_TEMP;
                            oven_data.oven_state = START;
                            break;
                        }
                        update_display(oven_data);
                        break;
                    }
                }
            }
        case COUNTDOWN:
            {
                while (oven_data.oven_state == COUNTDOWN) { // Needed to stay in state for timer.
                    // oven_data.initial_cook_time
                    // oven_data.cooking_time_left = oven_data.initial_cook_time*2
                    // twoHzFlag = FALSE;
                    if (twoHzFlag == TRUE && oven_data.cooking_time_left > 0) {
                        oven_data.cooking_time_left--;
                        twoHzFlag = FALSE;
                        update_display(oven_data);
                    }
                    if (twoHzFlag == TRUE && oven_data.cooking_time_left == 0) {
                        oven_data.oven_state = START;
                        twoHzFlag = FALSE;
                        update_display(oven_data);
                    }

                    // Check for exit button:
                    // Look to see if start is going:
                    if (buttonValues & BUTTON_EVENT_4DOWN) {
                        oven_data.oven_state = PENDING_RESET;
                        twoHzFlag = FALSE;
                        oven_data.button_press_counter = timerCounter;
                        break;
                    }
                }
                break;
            }
        case PENDING_RESET:
            {
                while (oven_data.oven_state = PENDING_RESET) {
                    // CHECK IF LONG PRESS HAS OCCURED.
                    if (((timerCounter - oven_data.button_press_counter) > LONG_PRESS) && buttonValues & BUTTON_EVENT_4UP) {
                        oven_data.oven_state = RESET;
                        twoHzFlag = FALSE;
                        update_display(oven_data);
                        break;
                    }
                    // Check if button has been released before long press counter.
                    if (((timerCounter - oven_data.button_press_counter) <= LONG_PRESS) && buttonValues & BUTTON_EVENT_4UP) {
                        oven_data.oven_state = COUNTDOWN;
                        twoHzFlag = FALSE;
                        update_display(oven_data);
                        break;
                    }
                    // Keep counting down.
                    if (twoHzFlag == TRUE && oven_data.cooking_time_left > 0) {
                        oven_data.cooking_time_left--;
                        twoHzFlag = FALSE;
                        update_display(oven_data);
                    }
                }
                break;
            }
        default:
            {
                oven_data.oven_state = RESET;
                break;
            }
        }
        }
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

// Function will just return total number of min in the given
// number of seconds.

uint32_t seconds_to_min(uint16_t in)
{
    // Given 16 bit number 'in' of range [256 - 0]:
    // Q = a/k  k = 60.
    // Q = (((uint16_t)A*(uint16_t)0x8889)>>16)>>5;
    return ((in * (uint32_t) 0x8889) >> 16) >> 5;
}

// Function that returns the current seconds count based on
// given seconds value. Sends return in 59 - 0 range.

uint16_t seconds_in_time(uint16_t in)
{
    return in % 60;
}

/*
 * Function that takes in the ovens_data struct and prints out the 
 * corresponding OLED display.
 * Use EX: update_display(oven_data);
 */
void update_display(ovens_data data)
{
    OledClear(OLED_COLOR_BLACK);
    // Create strings and pointers for character display.
    char ptOLED[200];
    char OLEDholder[100];
    char modeString[6];
    if (data.mode == BAKE) {
        strcpy(modeString, "Bake");
    }
    if (data.mode == TOAST) {
        strcpy(modeString, "Toast");
    }
    if (data.mode == BROIL) {
        strcpy(modeString, "Broil");
    }


    // Check what state the oven is in:
    // If the state is in one of the off modes print screen without 

    // IDLE SATE WAITING FOR SOMETHING NEW:
    if (data.oven_state == START || data.oven_state == PENDING_SEL_CHANGE) {
        // USING SPRINTF ADD EACH LINE TO DISPLAY CHAR. 
        // LINE 1:
        sprintf(ptOLED, "|%s|  Mode: %s \n", oven_OFF_top, modeString);

        // LINE 2:
        // Cook time functions:
        uint32_t minutes = seconds_to_min(data.initial_cook_time);
        uint16_t seconds = seconds_in_time(data.initial_cook_time);

        if (data.input_selection == TIME_SEL || data.mode == TOAST || data.mode == BROIL) {
            sprintf(OLEDholder, "|     | >Time: %d:%02d\n", (uint16_t) minutes, seconds);
        } else {
            sprintf(OLEDholder, "|     |  Time: %d:%02d\n", (uint16_t) minutes, seconds);
        }
        strcat(ptOLED, OLEDholder);

        // LINE 3:
        if (data.mode == TOAST) {
            sprintf(OLEDholder, "|-----|\n");
        } else {
            if (data.input_selection == TEMP_SEL && data.mode == BAKE) {
                sprintf(OLEDholder, "|-----| >Temp: %d\xF8%c\n", data.temperature, 'F');
            } else {
                sprintf(OLEDholder, "|-----|  Temp: %d\xF8%c\n", data.temperature, 'F');
            }
        }
        strcat(ptOLED, OLEDholder);

        // LINE 4:
        sprintf(OLEDholder, "|%s|", oven_OFF_bot);
        strcat(ptOLED, OLEDholder);
        // Update display
        OledDrawString(ptOLED);
        OledUpdate();

        // Update the LEDs
        uint8_t leds = 0x00;
        LEDS_SET(leds);
    }

    // OVEN BURNERS 'ON' and TIMER is running.
    if (data.oven_state == COUNTDOWN || data.oven_state == PENDING_RESET) {
        // USING SPRINTF ADD EACH LINE TO DISPLAY CHAR. 
        // LINE 1:
        if (data.mode == BAKE) {
            sprintf(ptOLED, "|%s|  Mode: %s \n", oven_ON_top, modeString);
        }
        if (data.mode == TOAST) {
            sprintf(ptOLED, "|%s|  Mode: %s \n", oven_OFF_top, modeString);
        }
        if (data.mode == BROIL) {
            sprintf(ptOLED, "|%s|  Mode: %s \n", oven_ON_top, modeString);
        }

        // LINE 2:
        // Cook time functions:
        uint32_t minutes = seconds_to_min(data.cooking_time_left >> 1);
        uint16_t seconds = seconds_in_time(data.cooking_time_left >> 1);

        if (data.input_selection == TIME_SEL || data.mode == TOAST || data.mode == BROIL) {
            sprintf(OLEDholder, "|     | >Time: %d:%02d\n", (uint16_t) minutes, seconds);
        } else {
            sprintf(OLEDholder, "|     |  Time: %d:%02d\n", (uint16_t) minutes, seconds);
        }
        strcat(ptOLED, OLEDholder);

        // LINE 3:
        if (data.mode == TOAST) {
            sprintf(OLEDholder, "|-----|\n");
        } else {
            if (data.input_selection == TEMP_SEL && data.mode == BAKE) {
                sprintf(OLEDholder, "|-----| >Temp: %d\xF8%c\n", data.temperature, 'F');
            } else {
                sprintf(OLEDholder, "|-----|  Temp: %d\xF8%c\n", data.temperature, 'F');
            }
        }
        strcat(ptOLED, OLEDholder);

        // LINE 4:
        if (data.mode == BAKE) {
            sprintf(OLEDholder, "|%s|", oven_ON_bot);
        }
        if (data.mode == TOAST) {
            sprintf(OLEDholder, "|%s|", oven_ON_bot);
        }
        if (data.mode == BROIL) {
            sprintf(OLEDholder, "|%s|", oven_OFF_bot);
        }
        strcat(ptOLED, OLEDholder);
        // Update display
        OledDrawString(ptOLED);
        OledUpdate();

        // Update the LEDs
        //LEDS_INIT();
        // data.cooking_time_left
        // data.initial_cook_time
        // Need to set the 8 LEDs scale to % of the time left vs initial time.
        uint16_t leds = 0x00;
        // Algorithm I figured out:
        // 8Y/X = current scaled value.
        // Y = Time remaining. X = Time initial.
        int remainder = data.cooking_time_left % 2;
        if (remainder == TRUE) {
            leds = (((data.cooking_time_left >> 1) * 8) / data.initial_cook_time);
            switch (leds) {
            case 8:
            {
                leds = led_8;
                break;
            }
            case 7:
            {
                leds = led_8;
                break;
            }
            case 6:
            {
                leds = led_7;
                break;
            }
            case 5:
            {
                leds = led_6;
                break;
            }
            case 4:
            {
                leds = led_5;
                break;
            }
            case 3:
            {
                leds = led_4;
                break;
            }
            case 2:
            {
                leds = led_3;
                break;
            }
            case 1:
            {
                leds = led_2;
                break;
            }
            case 0:
            {
                leds = led_1;
                break;
            }
            }
            LEDS_SET((uint8_t) leds); // 0x00 8 LEDs 1 bit for each.
        }


        

    }

    // Bake: Both heating elements are on.
    // Toast: Only bottom heating element is on.
    // Broil: Only top heating element is on.
    // Draw oven without any heating elements on

}


//The 2Hz timer is used exclusively for setting a flag when its interrupt
//occurs that can be checked in the state machine. 

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;
    twoHzFlag = TRUE;

}

//The 5Hz timer also sets an event flag when its interrupt occurs as well,
//but also increments the free running counter for use in distinguishing if
//a LONG_PRESS has occurred (where LONG_PRESS is a constant you will
//define with a value of 5, for representing 1 second of time). You will not
//be resetting this counter but instead be recording the time as part of
//the event. You can then check if the timing event has occurred by
//subtracting this count off the current count (i.e. (FreeCount-StartCount)
//>LONG_PRESS would indicate that a long press has occurred).

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    timerCounter++;

}

//The 100Hz timer should be used to check for button events exclusively.
//This makes sure the system is very responsive to button presses.

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    // Get current button values into 8-bit holder.
    buttonValues = ButtonsCheckEvents();

}


//Every morning I break my legs, and every afternoon 
//I break my arms. At night, I lie awake in agony 
//until my heart attacks put me to sleep.