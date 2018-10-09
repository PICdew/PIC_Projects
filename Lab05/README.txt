Created By: Kyle O'Rourke
Collaborators: Max Dunne
Date: 2/15/2018

Summary: 
In this lab, we created a doubly linked list using pointers and 
structs. Given an input of 10 char arrays, we had to put them 
into a list and then sort them first by length then by letter. 
An important part of the lab was understanding how pointers 
work, especially pointers that reference internal values like 
nextItem and previousItem. Figuring out that not all list 
locations will have a nextItem value was also important for 
list population. 

Methods: 
My approach to this lab was to first read the lab manual. I 
then started to fill out the functions in order of the given 
recommended lab approach listed in the manual. I then went 
through the .c file and made sure that each function was set 
up correctly. 
A huge challenge for this lab was getting the behavior for the 
append to list function to work correctly. I initially did not 
realize that the NULL case for a given input had to be accounted 
for. Ex: when there is only one element in the list and you want 
to append a new item using LinkedListCreateAfter(). 
To make the workflow easier, I used some of the given comments in 
the .h file for the .c file. As noted in the collaborator's
section and in my code, credit for some of the .c file comments 
is Max Dunne. If I could redo this lab, I would definitely have 
started sooner. I was not able to fully complete the sorting 
function.

Results:
The final functionality of the code shows that everything but 
the sorting algorithm is working correctly. I spent around 6 Hours 
on the lab and had to use some of my late hours. I liked 
how the lab taught me a bit about linked lists and sorting. 
I implemented my own type of sorting algorithm that instead of 
comparing one value to the entire list, it compares one value 
to its neighbor and swaps them if out of order. This way I 
didn't need to keep track of data size in an int. The hardest 
part of the lab was debugging my code. I used code breaks a lot.
The hardest error to debug was "runtime exception detected."
This happened when I was trying to operate on a NULL pointer in
the LinkedListCreateAfter() function. This would come up when I
just created the list and was adding a new value to it.


