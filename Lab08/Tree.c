/*
 * File:   Tree.c
 * Author: Kyle O'Rourke
 * 
 */

#include "Tree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>

// Example usage for creating a tree:
// char treeData[7] = "abdecfg"; // Not a string! Missing ending '\0'
// Node *root = TreeCreate(3, treeData); 

Node *TreeCreate(int level, const char *data)
{

    Node *newNode = (Node*) malloc(sizeof (Node));
    // Check if node space is open.
    if (newNode == NULL) {
        printf("\nMemory for new node not available!\n");
        return (NULL);
    }

    newNode->data = *data;

    if (level == 1) {
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    } else {
        newNode->leftChild = TreeCreate((level - 1), data + 1);
        newNode->rightChild = TreeCreate((level - 1), data + (1 << (level - 1)));
    }

    // Return should give a pointer to the head of the tree or NULL for failure.
    return (newNode);
}