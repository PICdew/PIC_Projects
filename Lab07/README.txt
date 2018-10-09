Lab 7
Created By: Kyle O'Rourke
Collaborators: None
Date: 2/28/2018


Summary:
In this lab, we created a virtual toaster oven using the PIC development board 
OLED peripheral. We had to implement a state machine that had five states. Each 
state performed a different function and controlled the behavior of the OLED. 
Transitioning between states activated timer flags and counter variables.

The program had to read a potentiometer on the development board and set either 
the time or temperature values according to the scaled potentiometer value. The 
max time possible is 4 min 16 seconds. The min temperature is 300 degrees F, 
the max temperature is 555 degrees F. When cooking, the virtual toaster had to 
display burners on either the top or bottom turning on depending on what cooking 
mode it was in.

Methods:
My approach to this lab was first to read the lab manual. I then went through 
and read each given h file for information on each library that I was supposed 
to use. I then started working on creating the display function. I first made 
the display function output just the toaster oven image with no function input 
or display text to test if my display prints were working correctly. I then made 
the state machine and state struct. Using sprintf to concatenate my print string 
to the OLED worked really well and made adding in variables much easier.

Results:
I was able to complete the entire lab as stated in the lab manual. This lab took 
around 14 hours to finish. I think I would have finished sooner if I focused more 
on getting it done while in the lab. I disliked the disorganization of the lab 
manual and the state machine spec. I think the state machine naming could have 
been done much better. (Reset)->(Idle)->{(Cook)->(Idle),(SettingsChange)->(Idle)}

