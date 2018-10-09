Kyle O'Rourke
Partner: Isaiah Barron 
Lab 9 - Battleboats
March 15, 2018

Summary:
In this lab we created a program that played out a game of battle boats over a two wire serial interface (RS232). The game displayed the player's boat information on the provided OLED display along with the opponent’s game information. The Pic devices would send encrypted values to determine who goes first. It would then play a game by sending coordinate data and response data back and forth between devices until one wins or loses.

Approach:
This lab was very challenging. To start, we individually read over the lab manual and tried to make sense of it. When that failed, we went to the TAs and Dunne. My partner worked on the message decoding state machine file while I worked on the field file. We then swapped places to see if we could spot any bugs in the other's code or to see if there is any way to improve the code. We could not get anything to parse at first but figured out eventually that we should read the UART every call to the state machine. Our teamwork and coordination worked very well, our software debugging did not. Since we started early and read the lab manual and went to the TAs for help and still turned it in on time that only thing I would do differently is not use so many late hours on previous labs (if that was ever a possibility). My partner was a joy to work with an we worked very well all throughout the lab. 

Results:
Our lab is completely functional and working as it is supposed to. My partner and I probably spent around 38 hours on this lab. I liked the overall game that we created. I did not like either of the parsing state machines. I would have liked to create my own state machines for this lab since the ones given were very poorly illustrated. I think this lab was worthwhile only for learning how to debug your code. Most everything else in the lab was stuff we already covered in previous labs. The lab manual was very hard to understand at first and it seems like the only way to understand it is to have someone who has previously done the lab (TAs) explain the vague parts. If the lab provided more design freedom, I think that it would have been a lot faster.
