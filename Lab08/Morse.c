/*
 * File:   Morse.c
 * Author: Kyle O'Rourke
 * 
 */

#include "Morse.h"
#include "Tree.h"
#include "BOARD.h"
#include "Buttons.h"

// Static globals:
static Node *root;
static Node *head;
static int state_timer;

static enum state {
    WAITING,
    DOT,
    DASH,
    INTER_LETTER
} State = WAITING;

// Defines:
#define DOT_TIME 25
#define DASH_TIME 50
#define INTERLETTER_TIME 100
#define INTERWORD_TIME 200

/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */
int MorseInit(void)
{
    // Morse levels = 6
    char MorseTreeData[] = {'#', 'E', 'I', 'S', 'H', '5', '4', 'V', '#', '3', 'U', 'F', '#', '#', '#', '#', '2', 'A', 'R', 'L',
        '#', '#', '#', '#', '#', 'W', 'P', '#', '#', 'J', '#', '1', 'T', 'N', 'D', 'B', '6', '#', 'X', '#', '#', 'K', 'C',
        '#', '#', 'Y', '#', '#', 'M', 'G', 'Z', '7', '#', 'Q', '#', '#', 'O', '#', '8', '#', '#', '9', '0'};
    head = TreeCreate(6, MorseTreeData);
    ButtonsInit();
    //State = WAITING;
    // Check if the root is bad.
    if (head == NULL) {
        return STANDARD_ERROR;
    } else {
        root = head;
        return SUCCESS;
    }

}

/**
 * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
 * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
 * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
 * decoding and return the decoded character. During that call to MorseDecode() the return value
 * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
 * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
 * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
 * then the internal state should be reset and STANDARD_ERROR should be returned.
 * 
 * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
 * 
 * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
 *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
 *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
 *         or `in` isn't a valid member of the MorseChar enum.
 */
char MorseDecode(MorseChar in)
{
    // Basically, given either MORSE_CHAR_DOT, MORSE_CHAR_DASH or MORSE_CHAR_END_OF_CHAR return
    // either SUCCESS, STANDARD_ERROR, or the current char value.
    // MORSE_CHAR_DECODE_RESET resets the current spot in the tree and returns SUCCESS.
    // Any invalid inputs returns STANDARD_ERROR.
    if (head == NULL) {
        return STANDARD_ERROR;
    }
    // root is the pointer to the current place on the tree.    
    switch (in) {
    case MORSE_CHAR_DOT:
    {
        // Move left:
        root = root->leftChild;
        if (root == NULL) {
            // Not a valid location.
            return STANDARD_ERROR;
        } else {
            // Valid location
            return SUCCESS;
        }
        break;
    }
    case MORSE_CHAR_DASH:
    {
        // Move right:
        root = root->rightChild;
        if (root == NULL) {
            // Not a valid location.
            return STANDARD_ERROR;
        } else {
            // Valid location.
            return SUCCESS;
        }
        break;
    }
    case MORSE_CHAR_END_OF_CHAR:
    {
        // Return current char value
        if (root->data == NULL) {
            return STANDARD_ERROR;
        } else {
            return root->data;
        }
        break;
    }
    case MORSE_CHAR_DECODE_RESET:
    {
        // Reset to top of tree.
        root = head;
        return SUCCESS;
        break;
    }
    default: return STANDARD_ERROR;
    }
}

/**
 * This function calls ButtonsCheckEvents() once per call and returns which, if any,
 * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
 * events in its input and should be called at 100Hz so that the timing works. The
 * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
 * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
 * (>= 1s), and words (>= 2s).
 *
 * @note This function assumes that the buttons are all unpressed at startup, so that the first
 *       event it will see is a BUTTON_EVENT_*DOWN.
 *
 * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.5s, and then waiting
 * will decode the string '.-' (A). It will trigger the following order of events:
 * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
 * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
 * 
 * @return The MorseEvent that occurred.
 */

// Basically returns any of the following values:
//      MORSE_EVENT_NONE,
//	MORSE_EVENT_DOT,            - Event when reading a dot: timer is >= 25
//	MORSE_EVENT_DASH,           - Event when reading a dash: timer is < 50
//	MORSE_EVENT_INTER_LETTER,   - Event when reading a new letter: 
//	MORSE_EVENT_INTER_WORD

MorseEvent MorseCheckEvents(void)
{
    // Get button values:
    uint8_t buttonVAL = ButtonsCheckEvents();

    // Implement the state machine here:
    switch (State) {
    case WAITING:
    {
        if (buttonVAL & BUTTON_EVENT_4DOWN) { // User pressed button
            State = DOT; // Change state to DOT.
            state_timer = 0; // Set timer to 0.
            return MORSE_EVENT_NONE;
            break;
        } else {
            state_timer = 0;
            return MORSE_EVENT_NONE;
            break;
        }
    }
    case DOT:
    {
        if ((buttonVAL & BUTTON_EVENT_4UP) && (state_timer < DASH_TIME)) { // DOT EVENT
            state_timer = 0;
            State = INTER_LETTER;
            return MORSE_EVENT_DOT;
            break;
        } else if (state_timer >= DASH_TIME) { // DASH EVENT
            state_timer = 0;
            State = DASH;
            return MORSE_EVENT_NONE;
            break;
        } else {
            // Do nothing. Wait.
            state_timer++;
            return MORSE_EVENT_NONE;
            break;
        }
    }
    case DASH:
    {
        if (buttonVAL & BUTTON_EVENT_4UP) { // If the button is released send dash event.
            state_timer = 0;
            State = INTER_LETTER;
            return MORSE_EVENT_DASH;
            break; // Just in case.
        } else {
            // Do nothing until button is up.
            State = DASH;
            state_timer = 0;
            return MORSE_EVENT_NONE;
            break;
        }
    }
    case INTER_LETTER:
    {
        // If time is less than one second before next action (button release/press)
        // then not a new letter.
        if ((buttonVAL & BUTTON_EVENT_4DOWN) && (state_timer < INTERLETTER_TIME)) {
            state_timer = 0;
            State = DOT;
            return MORSE_EVENT_NONE;
            break;
        }// If time is greater than one second but less than two seconds then new letter.
        else if ((buttonVAL & BUTTON_EVENT_4DOWN) && (state_timer >= INTERLETTER_TIME) && (state_timer < INTERWORD_TIME)) {
            // New letter but not new word:
            state_timer = 0;
            State = DOT;
            return MORSE_EVENT_INTER_LETTER;
            break;
        }// If time is greater than 2 seconds new word event.
        else if (state_timer >= INTERWORD_TIME) {
            state_timer = 0;
            State = WAITING;
            return MORSE_EVENT_INTER_WORD;
            break;
        } else {
            // Do nothing. Wait.
            state_timer++;
            return MORSE_EVENT_NONE;
            break;
        }
    }
    default:
    {
        State = WAITING;
    }
    }
    return MORSE_EVENT_NONE;
}