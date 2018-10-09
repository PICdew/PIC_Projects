
Kyle O'Rourke
ktorourk@ucsc.edu
2/1/2018

This lab took me about 12 hours to complete.

I think the reason this lab took so long for me was because I
didn't have a firm grasp on the concept of pointers and passing
arrays to functions.

My program would get compiler warnings for printf("%f", x)
saying that it is expecting a double and not a float. I think
Max or a TA said this should be fine.

Currently my Matrix inverse is failing tests with decimal points.

Below is the program output my software currently achieves:

PASSED (2/2): MatrixEquals()
PASSED (2/2): MatrixMultiply()
PASSED (2/2): ScalarMultiply()
PASSED (2/2): MatrixDeterminant()
PASSED (2/2): MatrixAdd()
PASSED (2/2): MatrixScalarAdd()
PASSED (2/2): MatrixTranspose()
PASSED (1/2): MatrixInverse()
PASSED (2/2): MatrixTrace()

Printing Array Using Function:
 --------------------
| 0.00 | 1.00 | 2.00 | 
 --------------------
| 3.00 | 4.00 | 5.00 | 
 --------------------
| 6.00 | 7.00 | 8.00 | 
 --------------------

Printing Array Hardcoded:
 --------------------
| 0.00 | 1.00 | 2.00 |
 --------------------
| 3.00 | 4.00 | 5.00 |
 --------------------
| 6.00 | 7.00 | 8.00 |
 --------------------