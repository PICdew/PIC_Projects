// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>

//CMPE13 Support Library
#include "UNIXBOARD.h"

// User libraries
#include "Game.h"
#include "Player.h"

// **** Set any macros or preprocessor directives here ****


// **** Declare any data types here ****
#define NORTH 'N'
#define EAST 'E'
#define SOUTH 'S'
#define WEST 'W'
#define QUIT 'Q'

// **** Define any global or external variables here ****
static const int SEP_TEXT = 10;

// **** Declare any function prototypes here ****
char GetUserInput();

int main()
{
    // TESTING THE GAME.C FUNCTIONS:
    //GameInit();

    // Test the c functions that he wanted us to make...
    /*char titleTest[GAME_MAX_ROOM_TITLE_LENGTH+1];
    GameGetCurrentRoomTitle(&titleTest);
    char descTest[GAME_MAX_ROOM_DESC_LENGTH+1];
    GameGetCurrentRoomDescription(&descTest);*/

    // Test the current exits function:
    /*uint8_t exitsAvai = GameGetCurrentRoomExits();
    printf("%i", exitsAvai);*/

    //printRoomTitle();
    //printRoomDescription();
    //TestPrintExits();
    //printInventory();

    // Testing the getchar function.
    // Idea:
    // Pull the first character, then attempt to read another, if it is NOT
    // a new line then we know they done f'd up.

    //return 0;
    // TESTS END.

    /******************************** Your custom code goes below here ********************************/

    if(!GameInit()){ // Setup the game values.
        FATAL_ERROR();
    }

    /** The while loop will have three main functions. Updating the display, checking for
     *  button presses, and acting on the button presses.
     */

	uint8_t gameEndFlag = FALSE;
    while(gameEndFlag == FALSE){

        /** UPDATE DISPLAY **/
		// Get display values.
		char title[GAME_MAX_ROOM_TITLE_LENGTH+1];
		GameGetCurrentRoomTitle(title);
		char description[GAME_MAX_ROOM_DESC_LENGTH+1];
		GameGetCurrentRoomDescription(description);
		
		printf( "\033[2J"); //clear the screen
		printf( "\033[H"); // Move cursor to top left.
		
		// Print the room title:
		printf("\033[44m");					  // Turns background blue.
		printf("\033[32m%s\033[0m", title); // Makes text green then normal again.
		// Print room description.
		printf( "\033[H"); // Move cursor to top left.
		printf("\033[1B"); // "newline"
		printf("\033[0m%s", description);  	  // Print out the room description:
		
		
		// Print North on compass:
		printf( "\033[H"); // Move cursor to top left.
		printf("\033[44B"); // Move the cursor lines down.
		printf("\033[3A"); // Move the cursor lines up.
		printf("\033[10C"); // Move cursor 20 lines over.
		if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS)){ // See if we should make it green or red.
			printf("\033[92mNORTH");
		}
		else{
			printf("\033[31mNORTH");
		}		
		printf("\033[2B"); // "newline"
		
		// Print West on compass:
		printf( "\033[H");  // Move cursor to top left.
		printf("\033[44B"); // Move the cursor lines down.
		printf("\033[2A"); // Move the cursor lines up.
		if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS)){ // See if we should make it green or red.
			printf("\033[92mWEST");
		}
		else{
			printf("\033[31mWEST");
		}
		
		// Print East on compass:
		if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS)){ // See if we should make it green or red.
			printf("\033[92m");
			printf("\033[17CEAST");
		}
		else{
			printf("\033[31m");
			printf("\033[17CEAST");
		}
		
		// Print South on compass:
		printf("\033[1B"); // "newline"
		printf("\033[15D"); // Move cursor 15 lines left.
		if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS)){ // See if we should make it green or red.
			printf("\033[92mSOUTH");
		}
		else{
			printf("\033[31mSOUTH");
		}
		
		
		// Add newline for user input.
		printf("\033[1B"); // "newline"
		printf("\033[25D");
		printf("\033[0m");
		
        /** CHECK FOR USER INPUT **/
        char userIn;
        userIn = GetUserInput();
        if(userIn == STANDARD_ERROR){   // Bad input!
            continue;   // Restart the loop since bad input was given.
        }

        /** ACT ON INPUT **/
        // Act on given input for each exit:
        switch(userIn) {
        case NORTH: {
            if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS)){
                // Go North:
                if(!GameGoNorth()){
                    // Error moving NORTH!
					printf("ERROR AT: MOVING NORTH MAIN.");
                    return STANDARD_ERROR;
                }
                break;
            }
            else {
                break; // Cannot leave that way!
            }
        } // NORTH END.
        case EAST: {
            if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS)){
                // Go East:
                if(!GameGoEast()){
                    // Error moving EAST!
					printf("ERROR AT: MOVING EAST MAIN.");
                    return STANDARD_ERROR;
                }
                break;
            }
            else {
                break;
            }
        } // EAST END.
        case SOUTH: {
            if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS)){
                // Go South:
                if(!GameGoSouth()){
                    // Error moving SOUTH!
					printf("ERROR AT: MOVING SOUTH MAIN.");
                    return STANDARD_ERROR;
                }
                break;
            }
            else {
                break;
            }
        } // SOUTH END.
        case WEST: {
            if((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS)){
                // Go West:
                if(!GameGoWest()){
                    // Error moving WEST!
					printf("ERROR AT: MOVING WEST MAIN.");
                    return STANDARD_ERROR;
                }
                break;
            }
            else {
                break;
            }
        } // WEST END.
        case QUIT: {
            // QUIT GAME!
			gameEndFlag = TRUE;
            break;
        }
        } // SWITCH END.



    } // WHILE TRUE END
	printf("\033[104m");
    printf("\033[93m\nGAME OVER: Thanks for playing and have a great spring break!");
	printf("\033[0m\n");
    return 0;
    /**************************************************************************************************/
}

/** GetUserInput
 * Grabs user input and does error checking. This is a blocking function!
 */
char GetUserInput(){
    char inputBuff;
    inputBuff = getchar();          // Wait for user input.
    if(inputBuff == '\n'){          // Check if the user input is a newline.
        return STANDARD_ERROR;
    }
    inputBuff = toupper(inputBuff); // Uppercase the input.
    if(getchar() != '\n'){          // This checks for more than one input.
        // Error, too many inputs.
        while(getchar() != '\n'){} // Clears out the buffer.
        return STANDARD_ERROR;
    }
    else if(inputBuff != NORTH && inputBuff != EAST && inputBuff != SOUTH && inputBuff != WEST && inputBuff != QUIT){
        // Error. Wrong value given.
        return STANDARD_ERROR;
    }
    else{
        // Hurray! Good inputs.
        return inputBuff;
    }
}
