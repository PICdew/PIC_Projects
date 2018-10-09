// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "MatrixMath.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
void testsPassed(int x);

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    // **** TESTING ARRAYS ****
    float testArray1[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    float testArray2[3][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    float testArray3[3][3] = {
        {3, 0, 2},
        {2, 0, -2},
        {0, 1, 1}
    };
    float testArray4[3][3] = {
        {0.5, 1, 3},
        {-1, -2, -1.5},
        {-1, 1, -2}
    };
    float testArray5[3][3] = {
        {0, -1, -2},
        {-3, -4, -5},
        {-6, -7, -8}
    };
    float resultArray[3][3];

    // **** FUNCTION TESTS ****
    int tPass = 2; // Test pass counter

    // MATRIX EQUALS:
    //
    if (MatrixEquals(testArray1, testArray1) != 1) {
        tPass--;
    }
    if (MatrixEquals(testArray1, testArray2) != 0) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixEquals()");
    tPass = 2;


    // MATRIX MULTIPLY:
    //
    // Known matrix results:
    float mulResult1[3][3] = {
        {9, 12, 15},
        {9, 12, 15},
        {9, 12, 15}
    };
    float mulResult2[3][3] = {
        {21, 25.5, 30},
        {-15, -19.5, -24},
        {-9, -11, -13}
    };
    // TEST 1
    MatrixMultiply(testArray1, testArray2, resultArray);
    if (MatrixEquals(resultArray, mulResult1) != 1) {
        tPass--;
    }
    // TEST 2
    MatrixMultiply(testArray4, testArray2, resultArray);
    if (MatrixEquals(resultArray, mulResult2) != 1) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixMultiply()");
    tPass = 2;


    // SCALAR MULTIPLY:
    //
    int scalarTestInt = 1;
    // TEST 1
    MatrixScalarMultiply(scalarTestInt, testArray1, resultArray);
    if (MatrixEquals(resultArray, testArray1) != 1) {
        tPass--;
    }
    // TEST 2
    scalarTestInt *= -1;
    MatrixScalarMultiply(scalarTestInt, testArray2, resultArray);
    if (MatrixEquals(resultArray, testArray5) != 1) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "ScalarMultiply()");
    tPass = 2;


    // MATRIX DETERMINANT:
    //
    if (MatrixDeterminant(testArray2) != 0.0) {
        tPass--;
    }
    if (MatrixDeterminant(testArray3) != 10.0) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixDeterminant()");
    tPass = 2;


    // MATRIX ADD:
    //
    float addResult1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float addResult2[3][3] = {
        {1, 0, -1},
        {-2, -3, -4},
        {-5, -6, -7}
    };
    MatrixAdd(testArray1, testArray2, resultArray);
    if (MatrixEquals(resultArray, addResult1) != 1) {
        tPass--;
    }
    MatrixAdd(testArray1, testArray5, resultArray);
    if (MatrixEquals(resultArray, addResult2) != 1) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixAdd()");
    tPass = 2;


    // MATRIX SCALAR ADD:
    //
    int scalarAddTestInt = -1;
    float addScalarResult1[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float addScalarResult2[3][3] = {
        { 2,  1,  0},
        {-1, -2, -3},
        {-4, -5, -6}
    };
    MatrixScalarAdd(scalarAddTestInt, testArray1, resultArray);
    if (MatrixEquals(resultArray, addScalarResult1) != 1) {
        tPass--;
    }
    scalarAddTestInt = 2;
    MatrixScalarAdd(scalarAddTestInt, testArray5, resultArray);
    if (MatrixEquals(resultArray, addScalarResult2) != 1) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixScalarAdd()");
    tPass = 2;

    
    // MATRIX TRANSPOSE:
    //
    float transposeResult1[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    float transposeResult2[3][3] = {
        { 0, -3, -6},
        {-1, -4, -7},
        {-2, -5, -8}
    };
    MatrixTranspose(testArray1, resultArray);
    if (MatrixEquals(resultArray, transposeResult1) != 1) {
        tPass--;
    }
    MatrixTranspose(testArray5, resultArray);
    if (MatrixEquals(resultArray, transposeResult2) != 1) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixTranspose()");
    tPass = 2;
    
    
    // MATRIX INVERSE:
    //
    float inverseTestM[3][3] = {
        {2, -1,  0},
        {1,  3, -1},
        {-3, 0,  1}
    };
    float inverseResult1[3][3] = {
        {0.2, 0.2, 0},
        {-0.2, 0.3, 1},
        {0.2, -0.3, 0}
    };
    float inverseResult2[3][3] = {
        {0.75, 0.25, 0.25},
        {0.5, 0.5, 0.5},
        {2.25, .75, 1.75}
    };
    MatrixInverse(testArray3, resultArray);
    if (MatrixEquals(resultArray, inverseResult1) != 1) {
        tPass--;
    }
    MatrixInverse(inverseTestM, resultArray);
    if (MatrixEquals(resultArray, inverseResult2) != 1) {
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixInverse()");
    tPass = 2;
    
  
    // MATRIX TRACE:
    //
    if(MatrixTrace(testArray1) != 3){
        tPass--;
    }
    if(MatrixTrace(testArray4) != -3.5){
        tPass--;
    }
    testsPassed(tPass);
    printf("%s", "MatrixTrace()");
    tPass = 2;
    
    
    // MATRIX PRINT:
    //
    printf("%s", "\n\nPrinting Array Using Function:");
    matrixPrint(testArray2);
    printf("%s", "\nPrinting Array Hardcoded:");
    printf("%s", "\n --------------------");
    printf("%s", "\n| 0.00 | 1.00 | 2.00 |");
    printf("%s", "\n --------------------");
    printf("%s", "\n| 3.00 | 4.00 | 5.00 |");
    printf("%s", "\n --------------------");
    printf("%s", "\n| 6.00 | 7.00 | 8.00 |");
    printf("%s", "\n --------------------");
    
//   --------------------
//  | 0.00 | 1.00 | 2.00 | 
//   --------------------
//  | 3.00 | 4.00 | 5.00 | 
//   --------------------
//  | 6.00 | 7.00 | 8.00 | 
//   --------------------

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}

void testsPassed(int x)
{
    printf("\nPASSED (%d/2): ", x);
}
