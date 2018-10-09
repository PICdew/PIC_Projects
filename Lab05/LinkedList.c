/*
 * File: LinkedList.C
 * By Kyle O'Rourke and Max Dune (Many of the file comments are from the author of LinkedList.h)
 *
 * Created on 2/14/2018
 *
 */

#include "LinkedList.h"
#include "limits.h"
#include "BOARD.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Creates new linked list.

// This function starts a new linked list. Given an allocated pointer to data it will return a
// pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
// returns NULL otherwise it should return a pointer to this new list item. data can be NULL.

// param: data The data to be stored in the first ListItem in this new list. Can be any valid
//             pointer value.
// return: A pointer to the malloc()'d ListItem. May be NULL if an error o cured.

ListItem *LinkedListNew(char *dataX)
{
    ListItem *newList = (ListItem*) malloc(sizeof (ListItem));

    if (newList == NULL) {
        return NULL;
    }

    newList->previousItem = NULL;
    newList->nextItem = NULL;
    newList->data = dataX;

    return newList;
}

// #2 Function to work on.
// This function allocates a new ListItem containing data and inserts it into the list directly
// after item. It rearranges the pointers of other elements in the list to make this happen. If
// passed a NULL item, LinkedListCreateAfter() should still create a new ListItem, just with no
// previousItem. It returns NULL if it can't malloc() a new ListItem, otherwise it returns a
// pointer to the new item. The data parameter is also allowed to be NULL.

// param: item The ListItem that will be before the newly-created ListItem.
// param: data The data the new ListItem will point to.
// return: A pointer to the newly-malloc()'d ListItem.

ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    ListItem *newItem = LinkedListNew(data); // Allocate new data.
    if (newItem == NULL) {
        return NULL;
    }

    if (item == NULL) { //create a new ListItem, with no previousItem.
        //newItem->previousItem = NULL;
        //newItem->nextItem = NULL;
        //newItem->data = data;
        return newItem;
    }

    // So new data was able to be allotted, begin insertion:
    // Step 1: Get new item address value: newItem, item
    // Step 2: Set pre and post address values in new item:
    newItem->nextItem = item->nextItem; // Sets new values next item to previous values next item.
    item->nextItem = newItem; // Set item's next item to the new item.
    newItem->previousItem = item; // Sets new item's previous value to item.

    // Step 3: Change pre and post address values.
    if (newItem->nextItem != NULL) { // Don't worry about nextItem's previous item.
        newItem->nextItem->previousItem = newItem;
    }

    //    ListItem *temp = newItem;   // Create temporary pointer to new item.
    //    temp = temp->nextItem;      // Set temporary pointer to next location.
    //    temp->previousItem = newItem;
    //    
    return newItem;

}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * param: list, An element in a list.
 * return: The first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list == NULL) {
        return NULL;
    }
    // To find the head of a list the following steps are required:
    // Step 1: Check if previous item point == NULL.
    // Step 2: If previous item location != NULL, acquire new previous item location.
    // Step 3: Loop back to step 1.
    //ListItem *temp = list;
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * param: list An item in the list to be sized.
 * return: The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list)
{

    if (list == NULL) {
        return 0; // "A NULL argument will result in 0 being returned."
    }

    ListItem *temp = LinkedListGetFirst(list); // Set a pointer temp = to list head.
    //printf("\n%s", temp->data);

    int i = 1; // Start counter at one since list will always have at least one value.
    while (temp->nextItem != NULL) { // Go through list until end.
        temp = temp->nextItem;
        //printf("\n%s", temp->data);
        i++;
        if (i >= (INT_MAX - 1)) { // Check if there is an out of range error.
            return 0;
        }
    }

    return i;
}

/**
 * LinkedListPrint() prints out the complete list to stdout.
 * param: list, Any element in the list to print.
 * return: SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list)
{

    if (list == NULL) {
        return STANDARD_ERROR; // NULL argument will result in STANDARD_ERROR (0) being returned.
    }

    ListItem *temp = LinkedListGetFirst(list); // Set a pointer temp = to list head.
    int listSize = LinkedListSize(list); // Find list size.
    //printf("List Size %d", listSize);

    printf("\n[");
    while (listSize > 0) {
        if (listSize != 1) {
            printf("%s, ", temp->data); // Print data with comma.
        } else {
            printf("%s", temp->data); // Last item, no comma.
        }

        temp = temp->nextItem; // Set temp to next item in list.
        listSize--; // Increment the count for list.
    }
    printf("]\n");

    return SUCCESS; // Return 1.
}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * param: item, The ListItem to remove from the list.
 * return: The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item)
{
    if (item == NULL) {
        return NULL;
    }

    // This function is almost the opposite of createAfter. Take given item and remove.
    // Step 1: Set previous item's nextItem value to removed item's nextItem value.
    // Step 2: Set nextItem's previousItem value to current item's previousItem.
    // ListItem *tempNextItem = item->nextItem;
    // ListItem *tempPrevItem = item->previousItem;
    item->nextItem->previousItem = item->previousItem; // Sets new values next item to previous values next item.
    item->previousItem->nextItem = item->nextItem;

    return item->data; // Return data pointer from item.
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. It is used
 * within LinkedListSort() for swapping items, but probably isn't too useful otherwise. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * param: firstItem, One of the items whose data will be swapped.
 * param: secondItem, Another item whose data will be swapped.
 * return: SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{

    if (firstItem == NULL || secondItem == NULL) {
        return 0; // This function should return STANDARD_ERROR (0) if either arguments are NULL.
    }

    // Swap the pointers for the first and second item data pointers.
    ListItem *temp = firstItem;
    firstItem->data = secondItem->data;
    secondItem->data = temp->data;

    return 1; // Return SUCCESS.

}

/**
 * LinkedListSort() performs a selection sort on list to sort the elements into decending order. It
 * makes no guarantees of the addresses of the list items after sorting, so any ListItem referenced
 * before a call to LinkedListSort() and after may contain different data as only the data pointers
 * for the ListItems in the list are swapped. This function sorts the strings in ascending order
 * first by size (with NULL data pointers counting as 0-length strings) and then alphabetically
 * ascending order. So the list [dog, cat, duck, goat, NULL] will be sorted to [NULL, cat, dog,
 * duck, goat]. LinkedListSort() returns SUCCESS if sorting was possible. If passed a NULL pointer
 * for either argument, it will do nothing and return STANDARD_ERROR.
 *
 * param: list Any element in the list to sort.
 * return: SUCCESS if successful or STANDARD_ERROR is passed NULL pointers.
 */
int LinkedListSort(ListItem *list)
{
    // Basically, sort the list by length then alphabetically using selection sort.

    if (list == NULL) {
        return STANDARD_ERROR; // NULL argument will result in STANDARD_ERROR (0) being returned.
    }
    list = LinkedListGetFirst(list);
    int listSize = LinkedListSize(list); // Find list size.

    // Sort by string size:
    int i;
    int cleanRun = 0;
    while (cleanRun == 0) {
        ListItem *temp = list; // Set a pointer list = to list head.
        ListItem *tempNP = temp->nextItem; // Get scrolling pointer.

        cleanRun = 1;
        for (i = 0; i < listSize - 1; i++) {
            if (strlen(tempNP->data) < strlen(temp->data)) { // Next value smaller, swap.
                LinkedListSwapData(tempNP, temp);
                cleanRun = 0; // Clean run did not occur, set value.
            }
            tempNP = tempNP->nextItem; // Increment current values
            temp = temp->nextItem;
        }
    }

    // Sort alphabetically:


}

/*
 * Create a new algorithm for counting occurrences of words within a
 * function named SortedWordCount() (whose definition follows from
 * UnsortedWordCount()) that uses the knowledge that it?s processing a
 * presorted word list to be more efficient than UnsortedWordCount().
 * You should first read and understand how UnsortedWordCount() works
 * before attempting to write SortedWordCount() so that you can be sure
 * that yours is faster. To think of how fast an algorithm is, just consider
 * how many times it accesses each element in the list.
 * Be sure to check that your word count handles the empty string
 * and no-string (NULL) cases properly!
 */
void SortedWordCount()
{

}







