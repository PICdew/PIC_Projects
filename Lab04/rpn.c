// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>
#include <string.h>
#include <stdlib.h>
// User libraries
#include "Stack.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int StackPush(struct Stack *stack, float value);
int StackIsFull(const struct Stack *stack);
int StackIsEmpty(const struct Stack *stack);
int StackPop(struct Stack *stack, float *value);
int StackGetSize(const struct Stack *stack);
void startDelay();
// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main()
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB4_TESTING
    BOARD_Init();
    /******************************** Your custom code goes below here *******************************/
    int i;
    for (i = 0; i < 10000; i++) {
    } // Fixes some issues with nonsense serial data at startup.
    printf("Welcome to Kyle's RPN calculator.\n");

    while (1) {
        // Create stack:
        struct Stack myStack;
        StackInit(&myStack);

        // Printer instructions:
        printf("\nEnter floats and + - /  * in RPN format:\n");
        printf("> ");

        // Get user input:
        char userInput[61];
        fflush(stdin);
        fgets(userInput, 60, stdin);

        // Find '\n' using strcspn and replace with '\0':
        userInput[strcspn(userInput, "\n")] = '\0';

        // Cut string into parts and add to the stack:
        //int stackFreqCheck = 0;
        char *inPtr;
        for (inPtr = strtok(userInput, " "); inPtr != NULL; inPtr = strtok(NULL, " ")) {
            // inPtr points to new location in input.


            // Valid operators and values:
            char validInputs[] = {'+', '-', '*', '/', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
            int valInputsLen = strlen(validInputs); // Gets length of valid inputs.
            int tokenLength = strlen(inPtr);
            char *tokPtr; // Pointer to a char.
            tokPtr = inPtr; // tempPtr now points to start of newest token string.

            // Now process the string as an input or operator and check for bad input.
            // Compare each character in the token string to every character in the validInputs string.
            int i = 0;
            int validCheck = 0;
            int operatorCheck = 0;
            for (i = 0; i < tokenLength; i++) { // Looks for end of the token string.
                int j = 0;
                validCheck = 0;
                for (j = 0;
                        j < valInputsLen; j++) { // While loop checks the character with all of validInputs.
                    if (validInputs[j] == *tokPtr) {
                        validCheck = 1;
                        operatorCheck = j;
                        break; // Character usable. Return success.
                    }
                }
                if (validCheck != 1) { // Characters not acceptable return error.
                    validCheck = 0;
                    break;
                }

                tokPtr += 1; // Increment the pointer location so that the function moves to the next location.
            }

            // printf("\nINPUT STATUS: %d  %s %d", validCheck, inPtr, operatorCheck); // Print token inputs and status.

            // ADD VALUES TO STACK:
            if (validCheck == 1) { // Check if the token is correct then push to stack.
                // Check if operator or number:
                // If operator, pop from stack and do action.
                if (operatorCheck <= 3) { // is operator, push with no change.
                    float val;
                    float *fp;
                    fp = &val;
                    StackPop(&myStack, fp);
                    float a = *fp;
                    fp = &val;
                    StackPop(&myStack, fp);
                    float b = *fp;
                    float result;
                    if (operatorCheck == 0) { // +
                        result = a + b;
                    }
                    if (operatorCheck == 1) { // -
                        result = a - b;
                    }
                    if (operatorCheck == 2) { // *
                        result = a * b;
                    }
                    if (operatorCheck == 3) { // div
                        result = a / b;
                    }
                    if (StackPush(&myStack, result) == STANDARD_ERROR) {
                        // Problem, send error.
                    }


                } else { // If number, create float.
                    float val = atof(inPtr);
                    if (StackPush(&myStack, val) == STANDARD_ERROR) {
                        // Problem, send error.
                    }
                }
            }

            //puts(inPtr);  // Prints the current token.
        }

        float val;
        float *fp;
        fp = &val;
        StackPop(&myStack, fp);
        float a = *fp;
        // Print result:
        printf("RESULT: %f", a);
    }
    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}


