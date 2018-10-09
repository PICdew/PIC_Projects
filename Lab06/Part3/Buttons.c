// Lab 6 part 3 buttons file.
// By: Kyle O'Rourke
// 

// Header includes:
#include "Buttons.h"


// ButtonsInit():
// Sets button registers to inputs:

void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}


// Function for checking the button activity:
//    BUTTON_EVENT_NONE = 0x00,
//    BUTTON_EVENT_1UP = 0x01,
//    BUTTON_EVENT_1DOWN = 0x02,
//    BUTTON_EVENT_2UP = 0x04,
//    BUTTON_EVENT_2DOWN = 0x08,
//    BUTTON_EVENT_3UP = 0x10,
//    BUTTON_EVENT_3DOWN = 0x20,
//    BUTTON_EVENT_4UP = 0x40,
//    BUTTON_EVENT_4DOWN = 0x80

uint8_t ButtonsCheckEvents(void)
{
    // Uses the BUTTON_STATES() macro. Returns 8-bit value 0000 XXXX
    // This value will have a high (1) bit for a pressed button and a
    // low (0) bit for not pressed.

    // Function tracks the state of the buttons, and if
    // 4 changes in a row happen it will output a change.
    // Ex: btn1 = 0 then for 4 samples btn1 = 1, output
    // a change in button state. Function will only start
    // the 4 samples if one original sample indicated a 
    // change in button state. 5 total samples.

    // Function returns an 8-bit int.
    static uint8_t btnReturn = 0x00; // Will be equal to output of the 4 read loop.

    // Get new button value.
    uint8_t btnNEW;
    btnNEW = BUTTON_STATES();

    // Check if new value is different from old value:
    if (btnReturn != btnNEW) { // Start 4 reading loop.
        int i;
        for (i = 0; i <= BUTTONS_DEBOUNCE_PERIOD; i++) { // 4 loop sample
            uint8_t j = BUTTON_STATES();
            if (btnNEW != j) { // Original change vs current vals.
                // Bouncing. Leave and return old value.
                return BUTTON_EVENT_NONE;
            }
            // Else: Do nothing and return new value out of loop.
        }

        // SO NEW BUTTON VALUE HAS OCCURED, FIGURE OUT WHICH ONE AND RETURN IT:

        // Compare the 8-bits of btnReturn (old) to btnNEW (new).
        // Any bit difference will tell us what the change is:
        /*
        uint8_t cmpFlag = btnReturn ^ btnNEW;   // Compare holds bits that are high for new changes.
        
        // If flag = 1, btn1 change only
        if(cmpFlag == 1){
            // Find out what change:
            
        }
        // If flag = 2, btn2 change only
        // If flag = 4, btn3 change only
        // If flag = 8, btn4 change only
        
        // If flag = 3, btn1 and btn2 change
        // If flag = 5, btn1 and btn3 change
        // If flag = 6, btn2 and btn3 change
        // If flag = 7, btn1 and btn2 and btn3 change
        // If flag = 9, btn1 and btn4 change
        // If flag = 10, btn2 and btn4 change
        // If flag = 11, btn1 and btn2 and btn4 change
        // If flag = 12, btn3 and btn4 change
        // If flag = 13, btn1 and btn3 and btn4 change
        // If flag = 14, btn2 and btn3 and btn4 change
        // If flag = 15, btn1 and btn2 and btn3 and btn4 change
        
        
        btnReturn = btnNEW;
        return XXX;
         */
    } else { // Nothing happened with the buttons return nothing.
        return BUTTON_EVENT_NONE;
    }



    return BUTTON_EVENT_NONE; // END Catch-all
}