Name: Kyle O’Rourke
Date: March 22, 2018
Lab 10

Summary:
In this lab we created a text document based game. The game would read a text file from a given folder and would parse the file according to the current player items. If the user had a specific number of items, the room that they are in would change upon arrival. This game is based off the book series Game of Thrones.

Methods:
To finish this lab, I first read the lab manual and worked out what functions I should work on first. I started with the player.h functions since they are the quickest to implement. I then worked on the file management in the Game.h functions. The hardest part of this lab was figuring out how to skip over sections of the file if you did not have all the items necessary for the room. I figured out that I could recursively call my parsing function that I created in the game file to make the code cleaner. I then worked on the rpg.c file after debugging every other function.

Results:
I was able to finish every part of the lab according to the rubric on Canvas and in the lab manual. My program adjusts the compass according to the screen size and outputs messages in the appropriate colors. This lab took around 12 hours to complete. 
I had extra time to add an Easter egg at the end of the program when the user quits.

NOTES:
In the Game.c file I created a parse helper function that opens the files. This function is ONLY called in the GameGo* functions. I was told this was a valid method for file access by Max and that no points would be deducted.
