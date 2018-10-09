Created By: Kyle O'Rourke
Date: 2/8/2018

Summary:
In this lab, we created an RPN calculator. The program takes in user
input as a single string and then calculates and displays the result
in the terminal. An important part of the lab was that the inputs are
brought in using fgets and are parsed before they are sorted and applied.
The parsed inputs are saved to a stack that holds the current running
result and input values. This allows us to have a long string of inputs
such as "1 6 + 4 8 - * 0*" be equivalent to "(1+6)*(4-8)*0". 

Approach (methods):
My approach to this lab was very similar to what was given in the lab
manual. I first created the stack.c file and each function that it is
supposed to contain. I then worked on the main file rpn.c and focused
a majority of the time on acquiring user input and parsing good input 
from bad input. A big problem that I kept having was the serial port
connection. I was not able to send the uC32 board any input without 
causing a lot of errors with the serial interface. After spending about
6 hours trying to get it to work I gave up and ran the code in CLion.
I was then able to get the correct behavior out of fgets and fprint.
I then worked on tokenizing the input string using strcspn and strtok.
Using strcspn, I was able to terminate the C string with a '\0' so that
strtok and other functions could properly use the input string. With
strtok, I tokenized the string so that each input separated by a ' ' 
would become its own string. Using a for loop I jumped through each
tokenized string by incrementing a pointer at each char in the string.
This allowed me to compare every char in each token to a set of valid
input chars.
{'+','-','*','/','.','0','1','2','3','4','5','6','7','8','9','\0'}
This made it easy to detect when an input was not a valid char. I then
worked on the actual calculation part of the lab within the same for
loop. This is where I had issues with setting the correct pointer values
to the stack functions.

Results:
The final capability of the program is faulty since I am unable to run
it on the intended uC32 device or an equivalent simulator. I was able 
to test a lot of the parsing capability using a different compiler (C98)
so I am marginally confident that will at least work as expected. It took
about 12 hours to complete the lab in total. I enjoyed learning about
pointers and how to use them to parse user inputs. I disliked the difficulty
in getting the serial communication with the uC32 to work. I spent a lot
of time (6-8 hours) on that issue which ended up nowhere. The lab manual
was very helpful with a majority of the lab (excluding the serial com).
If I could re-do this lab, I would focus a lot more on the functions for
error checking and stack manipulation and I would not have wasted so much
working on the serial issues.




