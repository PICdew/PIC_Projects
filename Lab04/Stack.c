/* 
 * Lab 4
 * File:   Stack.c
 * Author: Kyle O'Rourke
 *
 * Created on February 7, 2018
 * 
 */

#include "Stack.h"


/**
 * This function initializes the stack. For a statically allocated stack such
 * as this, this just means initializing currentItemIndex to -1 and setting
 * initialized to TRUE.
 */
void StackInit(struct Stack *stack){
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}


/**
 * This function checks for whether the stack is empty or not. In this simple
 * case this merely checks if the currentItemIndex == -1. Returns TRUE if the
 * stack is empty, FALSE if not. Also needs to check if the stack is
 * initialized and return FALSE if it isn't.
 */
int StackIsEmpty(const struct Stack *stack){
    if(stack->currentItemIndex != -1){
        return 0;
    }
    if(stack->initialized != TRUE){
        return 0;
    }
    return 1;
}


/**
 * This function checks for whether the stack is empty or not. For this simple
 * implementation that is merely if the currentItemIndex == STACK_SIZE - 1. 
 * Returns TRUE if the stack is full and FALSE if not. Also returns FALSE if
 * the stack isn't initialized.
 */
int StackIsFull(const struct Stack *stack){
    // Check stack for initialization:
    if(stack->initialized != TRUE){
        return FALSE;
    }
    // Check is stack is full:
    if(stack->currentItemIndex == (STACK_SIZE - 1)){
        return TRUE;
    }
    else{
        return FALSE;
    }
}


/**
 * This function pushes a value onto the stack. It returns SUCCESS for success
 * and STANDARD_ERROR for a failure. A failure occurs when trying to push onto full or
 * non-initialized stacks. (SUCCESS and STANDARD_ERROR are declared in the BOARD.h header file.
 */
int StackPush(struct Stack *stack, float value){
    // Check stack for initialization:
    if(stack->initialized != TRUE){
        return STANDARD_ERROR;
    }
    
    // Check if stack is full:
    if(stack->currentItemIndex == (STACK_SIZE - 1)){
        return STANDARD_ERROR;
    }
    
    // Stack can accept new value, add value to stack:
    stack->stackItems[stack->currentItemIndex] = value;
    
    // Increment stack index location:
    (stack->currentItemIndex)++;
    return SUCCESS;
}


/**
 * This function returns the top of the stack into the value point location.
 * It also returns a success code: SUCCESS for success and STANDARD_ERROR for failure.
 * Failure occurs when Pop() is called on empty stacks or non-initialized
 * stacks.
 *
 * NOTE: When trying to return the output into the value variable, you'll need
 * to write it with an extra asterisk like:
 *   *value = 7.0;
 * This is because of pointers and K&R 5.1-5.2 should explain it. This is the
 * only time in your lab where you'll need to do something like that for an
 * assignment operation.
 */
int StackPop(struct Stack *stack, float *value){
    // Check if stack is initialized or empty:
    if(StackIsEmpty(&stack) == TRUE){
        // Fail
        return STANDARD_ERROR;
    }
    
    // Stack is not empty or non-initialized, get top value:
    stack->stackItems[stack->currentItemIndex] = *value;
    // Decrement stack index location:
    (stack->currentItemIndex)--;
    // Return SUCCESS:
    return SUCCESS;

}


/**
 * Returns the current size of the stack in terms of how many active elements
 * are in it. Returns an error of SIZE_ERROR for uninitialized stacks, 0 for
 * empty and initialized stacks, and the number of elements in the stack
 * otherwise. SIZE_ERROR is declared in the BOARD.h header file.
 */
int StackGetSize(const struct Stack *stack){
    // Check stack for initialization:
    if(stack->initialized != TRUE){
        return SIZE_ERROR;
    }
    // Return number of elements in stack:
    return (stack->currentItemIndex + 1);
    
}