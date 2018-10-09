// ArtificialAgent.c
// Kyle O'Rourke & Isaiah Barron
// 3/9/2018

#include "BOARD.h"
#include <stdint.h>
#include <stdlib.h>

#include "Agent.h"
#include "Field.h"
#include "Protocol.h"
#include "Oled.h"
#include "FieldOled.h"


// ------------------- Declare global variables ------------------- 
static Field myField;
static Field virtualField;
static Field enemyField;

// ------------------- Function Prototypes --------------------
static uint8_t getBoatSize(BoatType currentBoat);


// AgentInit():
// The game starts and the user presses buttons 1-4 to place their boats.
// BTN1 = Places the boat.
// BTN2 = Moves the boat down.
// BTN3 = Moves the boat left.
// BTN4 = Rotates the boat 90 degrees clockwise.
// If boat location is invalid only cursor is displayed.
void AgentInit(void) {
    // Setup buttons:
    ButtonsInit();
    uint8_t buttons;
    
    // Clear fields and setup game:
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);
    
    // Pick first boat to be placed and set size variable.
    BoatType currentBoat = FIELD_BOAT_SMALL; 
    uint8_t boatSize;
    
    // Set cursor location and direction:
    //uint8_t row = 0; // Position on the Y.
    //uint8_t col = 0; // Position on the X.
    BoatDirection dir = FIELD_BOAT_DIRECTION_EAST;
    
    // Should have own event loop where buttonCheckEvents is called and checked:
    while(TRUE){
        
        boatSize = getBoatSize(currentBoat); // Get boat size for placement.
        buttons = ButtonsCheckEvents();  // Check buttons each loop through.
        
        // Act on each button press accordingly
        if((buttons & BUTTON_EVENT_1UP) == 1){  // Button 1 places the boat:
            
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
        }
        if((buttons & BUTTON_EVENT_2UP) == 1){  // Button 2 moves the boat down:
            
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
        }
        if((buttons & BUTTON_EVENT_3UP) == 1){  // Button 3 moves the boat left:
            
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
        }
        if((buttons & BUTTON_EVENT_4UP) == 1){  // Button 4 rotates the boat:
            
            switch(dir){
                case FIELD_BOAT_DIRECTION_NORTH: {
                    dir = FIELD_BOAT_DIRECTION_EAST;
                }
                case FIELD_BOAT_DIRECTION_EAST: {
                    dir = FIELD_BOAT_DIRECTION_SOUTH;
                }
                case FIELD_BOAT_DIRECTION_SOUTH: {
                    dir = FIELD_BOAT_DIRECTION_WEST;
                }
                case FIELD_BOAT_DIRECTION_WEST: {
                    dir = FIELD_BOAT_DIRECTION_NORTH;
                }
            }
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
        }
        
    } // While loop end.
    
}


// Checks the current cursor location to make sure it is in game field.
// If it is about to leave the field it moves it to the correct in field spot.
static void cursorMove(uint8_t *row, uint8_t *col, uint8_t rowMove, uint8_t colMove){
    
    // Add the rowMove and colMove values to the row and col values
    uint8_t tempRow = row + rowMove;
    uint8_t tempCol = col + colMove;

    if((row > (FIELD_ROWS-1)) || (row < 0)){ // Check for space in the 

    }
    if((col > (FIELD_COLS-1)) || (col < 0)){
        // OUT OF SPACE.

    }
}

static uint8_t getBoatSize(BoatType currentBoat){
    uint8_t boatSize;
    switch(currentBoat){
        case FIELD_BOAT_SMALL:
            boatSize = FIELD_BOAT_LIVES_SMALL;
            break;
        case FIELD_BOAT_MEDIUM:
            boatSize = FIELD_BOAT_LIVES_MEDIUM;
            break;
        case FIELD_BOAT_LARGE:
            boatSize = FIELD_BOAT_LIVES_LARGE;
            break;
        case FIELD_BOAT_HUGE:
            boatSize = FIELD_BOAT_LIVES_HUGE;
            break;
    }
    return boatSize;
}
