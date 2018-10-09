// Lab10
// Player.c file
// Written by: Kyle O'Rourke

// Support Libraries:
//#include "UNIXBOARD.h"
#include <stdio.h>
#include <string.h>

// User libraries:
#include "Player.h"

// Function prototype:
void clearInventory();

//#define INVENTORY_SIZE 4
static struct playerInventory{
	uint8_t item[INVENTORY_SIZE];
	uint8_t inventory_count;
}inventory;

/*
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item){
	if(inventory.inventory_count < INVENTORY_SIZE){
		// Space available, add to inventory.
		// printf("Item added to inventory!\n");
		inventory.item[inventory.inventory_count] = item;
		inventory.inventory_count++;
		return SUCCESS;
	}
	else {
		// No space available:
		return STANDARD_ERROR;
	}
}

/*
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item){
	int i;
	for(i = 0; i < inventory.inventory_count; i++){
		if(inventory.item[i] == item){
			// Item found.
			return SUCCESS;
		}
	}
	// Item not found, return failure:
	return STANDARD_ERROR;
}


// Helper functions:
// Give inventory quantity.
uint8_t inventoryCount(){
    return inventory.inventory_count;
}
// Clears the inventory by reseting the counter.
void clearInventory(){
    inventory.inventory_count = 0;
}
void printInventory(){
    if(inventory.inventory_count != 0){
        int i;
        for(i = 0; i < inventory.inventory_count; i++){
            printf("Inventory Item %d: %d\n", (i+1),inventory.item[i]);
        }
    }
    else{
        printf("No items in inventory.\n");
    }

}
