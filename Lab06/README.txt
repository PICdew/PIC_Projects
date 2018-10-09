Created By: Kyle O'Rourke
Collaborators: None
Date: 2/15/2018

Battle Boats Partner: Isaiah Barron

Summary:
In this lab, we created three programs capable of controlling the behavior of 8 LEDs on a
PIC development board. The first program would pole the flip switches on the board and set
the "speed" of a moving ON LED. When the switches were off the leds would signal at the
fastest speed. When they were all ON the leds would signal at the slowest speed. Since it
was a 4-bit input the total speed settings were 16.

In the second part of the lab we created a program that would read the value of the 
potentiometer on the board and display the bounded value (0 - 1023) as well as a % total 
to the OLED display.

In the third part of the lab we needed to read each push button and flip switch to output
the correct LED states. When the a switch was LOW the corresponding button would turn on
two LEDs when pressed. When the switch was HIGH the corresponding button would turn on the
LEDs when released.

Methods:
My approach to this lab was to first read the lab manual. I then went through and read each
given h file for information on each library that I was supposed to use or create. I then 
started on the first section following the lab guidelines. After finishing the first section
I uploaded the files to dropbox and sequentially worked on the next modules.
I was not able to figure out what I was supposed to do initially with the interrupts in
the modules. I had to ask a TA what the lab manual was trying to convey. I also had issues
with the last lab section.

Results:
I was able to complete the first two lab sections perfectly but was stuck on the last one
when time ran out. I had to submit my lab with a non functioning part 3. This lab took around
8 hours to get to where I am currently. I disliked the disorganization of the lab manual.
It seems like each manual was a collection of notes and specs all thrown together to form a 
lab.


NOTES:
Lab 6 Notes:

Main project files: bounce_part1.c, bounce_part2.c, bounce_part3.c – contains main()
EC: bounce_ec.c

Broken into multiple parts (ex: labs 1-3).

Oled.h and .c are given as is - don't mess with them.

Overall notes:
--------------
-> LATE  register Each bit in this register controls the output of a pin: 1 means high and 0 
means low. 
(The LEDs are on pins E0 through E7 and a high output corresponds to the LED being enabled or on.)

-> TRISE register Turns pin into an input and clearing that bit (to 0) sets it to an output


PART 1:
-------
>Create the led.h file.

-> Header guards are required for file.
-> IN file add xc.h file.
-> using a struct (TimerResult)

-> No leds files given.
