// Lab 10
// Game.c
// Written by Kyle O'Rourke

// Standard libraries
#include <string.h>
#include <stdio.h>
#include <math.h>

// User Libraries
#include "Game.h"
#include "Player.h"

// Defines:

// Constants:
static const char fileTemplate[15] = {'R','o','o','m','F','i','l','e','s','/','r','o','o','m','\0'};
static const char txt[5] = {'.','t','x','t','\0'};

// Function prototypes:
char decryptChar(char in, uint8_t roomNum);
int fileParse(uint8_t rmNum);
void clearTitle();
void clearDes();

// "A struct is used in Game.c to hold the title, description, and all 4 exits
// for the current room."
static struct currentRoomData {
	char title[GAME_MAX_ROOM_TITLE_LENGTH+1]; // Plus one to account for all possible title lengths.
	uint8_t titleLength;
	char description[GAME_MAX_ROOM_DESC_LENGTH+1];
	uint8_t descLength;
	uint8_t roomNum; 	// Holds the current room number.
	uint8_t Nexit; // Will be OR'd with the available exits.
	uint8_t Eexit;
	uint8_t Sexit;
	uint8_t Wexit;
} currentRoom;



/**************************
 * Game.c functions below *
 **************************/


/**
 * Game start setup function. Set current room to starting room and load it.
 * Return SUCCESS if it succeeds and STANDARD_ERROR if it doesn't.
 */
int GameInit(void){

    //clearInventory(); // Clears user inventory. Function located in the Player.c file.
    currentRoom.Nexit = STARTING_ROOM;
    if(GameGoNorth()){
        return SUCCESS;
    }
    else{
        return STANDARD_ERROR;
    }
}


/**
 * Finds the current room's title.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return length of the title string.
 */
int GameGetCurrentRoomTitle(char *title){
    // Copy the title into the desc pointer location.
    strcpy(title, currentRoom.title);
    return currentRoom.titleLength;

}


/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc){

    // Copy the description into the desc pointer location.
    strcpy(desc, currentRoom.description);
    return currentRoom.descLength;
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) {
 *   // The current room has a west exit.
 * }
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void){
    uint8_t exitsPossible = 0;  // Returned value OR'd with enum values.

    // NORTH
    if(currentRoom.Nexit){
        exitsPossible = exitsPossible | GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    // EAST
    if(currentRoom.Eexit){
        exitsPossible = exitsPossible | GAME_ROOM_EXIT_EAST_EXISTS;
    }
    // SOUTH
    if(currentRoom.Sexit){
        exitsPossible = exitsPossible | GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    // WEST
    if(currentRoom.Wexit){
        exitsPossible = exitsPossible | GAME_ROOM_EXIT_WEST_EXISTS;
    }

    return exitsPossible;
}


// Room transition functions. Return SUCCESS if the room has an exit in the requested direction, and
// the room was loaded. Return STANDARD_ERROR if failed.
int GameGoNorth(void){
    if(!currentRoom.Nexit){     // Check if the room even is available.
        return STANDARD_ERROR;
    }
    if(fileParse(currentRoom.Nexit)){   // Room is an option. Move to it by parsing data.
        return SUCCESS;
    }
    else{
        return STANDARD_ERROR;      // Something happened parsing the file@
    }
}

int GameGoEast(void){
    if(!currentRoom.Eexit){
        return STANDARD_ERROR;
    }
    if(fileParse(currentRoom.Eexit)){
        return SUCCESS;
    }
    else{
        return STANDARD_ERROR;
    }
}

int GameGoSouth(void){
    if(!currentRoom.Sexit){
        return STANDARD_ERROR;
    }
    if(fileParse(currentRoom.Sexit)){
        return SUCCESS;
    }
    else{
        return STANDARD_ERROR;
    }
}

int GameGoWest(void){
    if(!currentRoom.Wexit){
        return STANDARD_ERROR;
    }
    if(fileParse(currentRoom.Wexit)){
        return SUCCESS;
    }
    else{
        return STANDARD_ERROR;
    }
}


/**************************
 *    HELPER FUNCTIONS    *
 **************************/


/** FILE PARSE ENTIRE FILE **
 * Parses an entire file given the room number.
 * Saves values found to the data Struct.
**/
int fileParse(uint8_t rmNum){

    /** PART 0:
     * Open the file and check for errors. Set room number to current room.
    **/
    currentRoom.roomNum = rmNum;

    char fileNameBuff[30]; // Buffer for the file name.
	sprintf(fileNameBuff, "%s%d%s", fileTemplate, rmNum, txt); // Appends the room number to the buffer string.

	// OPEN THE FILE OF LOCATION: fileNameBuff
    FILE *fp;
    fp = fopen(fileNameBuff, "rb"); // Open the file and set fp to the file location.
    if(fp == NULL){
        // Failed. Let the user know why.
        printf("File open failed in parse! File name attempted: %s\n", fileNameBuff);
        return STANDARD_ERROR;
    }

    /** PART 1:
     * Get the title data.
    **/

    clearTitle(); // Clear away old title data.
    // Get the length of the file title chunk:
	uint8_t length;
	length = fgetc(fp);
	length = decryptChar((char)length, rmNum);
	if(length == FALSE){
        // Error. Should never not have a title length.
        return STANDARD_ERROR;
	}

	// Now run through the file up to length:
	int i;
	char nextChar;
	for(i = 0; i < length; i++){
		nextChar = fgetc(fp);
		nextChar = decryptChar(nextChar, rmNum);
		currentRoom.title[i] = nextChar;
	}
	currentRoom.title[length] = '\0'; // Add terminating character.
    currentRoom.titleLength = length;

    // fp now is at the end of the title data in the file.

    /** PART 2:
     * Finds room version item requirements and checks current items. If the
     * current inventory matches the room version, then it uses the room
     * description for that version. Leaves loop to process the description.
    **/

    while(TRUE){
        uint8_t versionJump = FALSE; // Current version flag.

        // Read first value in the version item requirements.
        uint8_t requirements;
        requirements = fgetc(fp);
        requirements = decryptChar((char)requirements, rmNum);

        // Check if the room requires items.
        if(requirements == FALSE){
            // Parse current room data.
            // (Basically leave the loop and let the rest handle it.)
            break;
        }
        else {   // Check if we have the items in inventory.
            for(i = 0; i < requirements; i++){
                uint8_t currentItem = fgetc(fp);
                currentItem = decryptChar((char)currentItem, rmNum);
                if(!FindInInventory(currentItem)){
                    // Item not found. Move to next version.
                    versionJump = TRUE;
                }
            }
            if(versionJump == TRUE){
                /** Since we only used a flag in the above item requirements it
                 *  is not necessary to move the pointer to the start of description
                 *  it is already there.
                 *  Run through description.
                 */
                // Get description length.
                length = fgetc(fp);
                length = decryptChar((char)length, rmNum);
                if(length == FALSE){
                    // Error. Should never not have a description length.
                    printf("Error parsing description length!\n");
                    return STANDARD_ERROR;
                }
                for(i = 0; i < length; i++){
                    nextChar = fgetc(fp);
                }
                // File pointer now at items contained location.

                /** Run through items in room. **/
                length = fgetc(fp); // Holds the total number of items in the current room version.
                length = decryptChar((char)length, rmNum);
                for(i = 0; i < length; i++){ // Collect the items.
                    nextChar = fgetc(fp);
                }

                /** Run through exits. (Simply run the pointer 4 times.) **/
                nextChar = fgetc(fp);
                nextChar = fgetc(fp);
                nextChar = fgetc(fp);
                nextChar = fgetc(fp);
            } // Version jump sequence end.

            // All items needed present in inventory. Load current version.
            // (Basically leave the loop and let the rest handle it.)
            else if(versionJump == FALSE){
                break;
            }
        }
    } // Requirement Loop END.

    /** PART 3:
     * Pointer at the description location. Parse.
    **/
    clearDes(); // Clear previous description data.

    // Get length of the description segment.
    length = fgetc(fp);
	length = decryptChar((char)length, rmNum);
	if(length == FALSE){
        // Error. Should never not have a description length.
        return STANDARD_ERROR;
	}

    // Run through the description and save it to array.
    // Now run through the file up to length:
	for(i = 0; i < length; i++){
		nextChar = fgetc(fp);
		nextChar = decryptChar(nextChar, rmNum);
		currentRoom.description[i] = nextChar;
	}
	currentRoom.description[length] = '\0'; // Add terminating character.
	currentRoom.descLength = length;

    // File pointer now at items contained location.

    /** PART 4:
     * Parse the items contained section and add items to inventory if possible.
     * Follows the same data convention as the item requirements data. (8-bit)
    **/
    uint8_t itemsInRoom;    // Holds the total number of items in the current room version.
    itemsInRoom = fgetc(fp);
    itemsInRoom = decryptChar((char)itemsInRoom, rmNum);
    //printf("Items in room: %d\n", itemsInRoom); // Testing/debug.
    for(i = 0; i < itemsInRoom; i++){ // Collect the items.
        uint8_t roomItem;
        roomItem = fgetc(fp);
        roomItem = decryptChar((char)roomItem, rmNum);
        //printf("Item: %d\n", roomItem); // Testing/debug.
        AddToInventory(roomItem);
    }
    // File pointer now at exits location.

    /** PART 5:
     * Parse the exit location data. Next four byte values are
     * ordered as north, east, south, west. The value of each byte
     * indicates the room located in that direction. A zero value
     * indicates the direction is attached to no room.
    **/

    // NORTH
    currentRoom.Nexit = fgetc(fp);
    currentRoom.Nexit = decryptChar((char)currentRoom.Nexit, rmNum);

    // EAST
    currentRoom.Eexit = fgetc(fp);
    currentRoom.Eexit = decryptChar((char)currentRoom.Eexit, rmNum);

    // SOUTH
    currentRoom.Sexit = fgetc(fp);
    currentRoom.Sexit = decryptChar((char)currentRoom.Sexit, rmNum);

    // WEST
    currentRoom.Wexit = fgetc(fp);
    currentRoom.Wexit = decryptChar((char)currentRoom.Wexit, rmNum);

    // PARSE OVER.

    fclose(fp); // Close the file.
    return SUCCESS;
}

// Clears the title.
void clearTitle(){
	int i;
	for(i = 0; i < GAME_MAX_ROOM_TITLE_LENGTH; i++){ // Clear title.
		currentRoom.title[i] = '\0';
	}
	return;
}

// Clears the description.
void clearDes(){
	int i;
	for(i = 0; i < GAME_MAX_ROOM_DESC_LENGTH; i++){	// Clear description.
		currentRoom.description[i] = '\0';
	}
	return;
}

// Decrypts a char and returns it.
char decryptChar(char in, uint8_t roomNum){
	return (char)((in)^((uint8_t)DECRYPTION_BASE_KEY + roomNum));
}

// Only used in testing
void printRoomTitle(){
    printf("%s\n", currentRoom.title);
}

// Only used in testing
void printRoomDescription(){
    printf("%s\n", currentRoom.description);
}

// Only used in testing
void TestPrintExits(){
    printf("North Exit: %d\n", currentRoom.Nexit);
    printf("East  Exit: %d\n", currentRoom.Eexit);
    printf("South Exit: %d\n", currentRoom.Sexit);
    printf("West  Exit: %d\n", currentRoom.Wexit);
}
