Lab 8
Created By: Kyle O'Rourke
Collaborators: None
Date: 3/7/2018

Lab 9 Partner: Issiah Barron

Summary:
In this lab, we created a Morse translator that would take in slashes and dots
and would print out a corresponding character. We had to implement a state machine 
that had three states. The first state would wait for a button press. The second 
state would see how long the button was pressed to determine if it is a slash
or dot input. The third state would see if it is a new word or just a new letter

The program had to read button four 100 times a second for user input.

Methods:
My approach to this lab was first to read the lab manual. I then went through 
and read each given h file for information on each library that I was supposed 
to use or create. I started on the tree functions first. I tested populating the 
tree and printing out non button based decode inputs. This allowed me to verify
that the tree functions were working as expected before trying to work on the other
functions. I then started working on the Morse functions and last the main functions.

Results:
I was able to complete the entire lab as stated in the lab manual. This lab took 
around 6 hours to finish. I think I would have finished sooner if the state machine
diagram in the lab manual was more straight forward and easier to read. I disliked 
the disorganization of the lab manual because there was conflicting information in 
the .h file comments. I used the .h file comments in my .c file implementation. One
hard to solve bug I had was not initializing the buttonsInit function.


