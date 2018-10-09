// File: MatrixMath.c
// Author: Kyle O'Rourke & Max Dune
// Matrix Math Program Lib for 3x3 matrix.
// 1/31/2018

#include "MatrixMath.h"     // Adding the .h file.
#include <stdio.h>
#include <math.h>
/*
 * Matrices are defined in row-major order, so that the matrix:
 *   0 1 2
 *   3 4 5
 *   6 7 8
 * is represented by the array `float mat[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
 * mat[rows][columns]
 *   c  c  c
 * r
 * r
 * r
 * 
 */

// Begin functions: 

/**
 * MatrixPrint sends a 3x3 array to standard output with clean formatting.
 */
void matrixPrint(float mat[3][3])
{
    int i, j;
    printf("%s", "\n --------------------\n");
    for (i = 0; i < 3; ++i) {
        printf("%s", "| ");
        for (j = 0; j < 3; ++j) {
            printf("%-.2f", (mat[i][j]));
            printf("%s", " | ");
        }
        printf("%s", "\n --------------------\n");
    }
}

/**
 * MatrixMultiply performs a matrix-matrix multiplication operation on two 3x3
 * matrices and returns the result in the third argument.
 */
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int i, j;

    // Make sure that result starts at 0.
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = 0;
        }
    }

    // Each output is the multiplication of corresponding locations summed with row and column.
    // ex: result[1][1] = (mat1[1][1]*mat2[1][1]) + (mat1[1][2]*mat2[2][1]) + (mat1[1][3]*mat2[3][1])
    //                         row   col        row   col
    // Constants per op: mat1[CONST][VAR], mat2[VAR][CONST]
    int k = 0;
    for (i = 0; i < 3; ++i) { // First level of loop.
        for (j = 0; j < 3; ++j) { // Second level.
            result[i][j] = (mat1[i][k] * mat2[k][j]) + (mat1[i][k + 1] * mat2[k + 1][j]) + (mat1[i][k + 2] * mat2[k + 2][j]);
        } // PROOF FOR ABOVE EXPRESSION IN .IMG FILE IN REPO.
    }
}

/**
 * MatrixAdd performs a matrix addition operation on two 3x3 matrices and 
 * returns the result in the third argument.
 */
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    // Adds each value from mat1 to mat2 at same location.
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

/**
 * MatrixEquals checks if the two matrix arguments are equal. The result is
 * 0 if FALSE and 1 if TRUE to follow the standard C conventions of TRUE and
 * FALSE.   FP_DELTA
 */
int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (fabs(mat1[i][j] - mat2[i][j]) > FP_DELTA) {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * MatrixScalarMultiply performs the multiplication of a matrix by a scalar.
 * The result is returned in the third argument.
 */
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = mat[i][j] * x;
        }
    }
}

/**
 * MatrixScalarAdd performs the addition of a matrix by a scalar. The result
 * is returned in the third argument.
 */
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = mat[i][j] + x;
        }
    }
}

/**
 * MatrixDeterminant calculates the determinant of a matrix and returns the
 * value as a float.
 */ //        r  c

float MatrixDeterminant(float mat[3][3])
{
    float det;

    // Using shortcut method of finding 3x3 det.
    float a1 = mat[0][0] * mat[1][1] * mat[2][2];
    float a2 = mat[0][1] * mat[1][2] * mat[2][0];
    float a3 = mat[0][2] * mat[1][0] * mat[2][1];

    float b1 = mat[2][0] * mat[1][1] * mat[0][2];
    float b2 = mat[2][1] * mat[1][2] * mat[0][0];
    float b3 = mat[2][2] * mat[1][0] * mat[0][1];

    det = a1 + a2 + a3 - b1 - b2 - b3;

    return det;
}

/**
 * MatrixTrace calculates the trace of a matrix. The result is returned as a
 * float.
 */
float MatrixTrace(float mat[3][3])
{
    return (mat[0][0] + mat[1][1] + mat[2][2]);
}

/**
 * MatrixTranspose calculates the transpose of a matrix and returns the
 * result through the second argument
 */
void MatrixTranspose(float mat[3][3], float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) { // Cycle through rows
        for (j = 0; j < 3; ++j) { // Cycle through columns
            if (i == j) {
                result[i][j] = mat[i][j];
            } else {
                result[i][j] = mat[j][i];
            }
        }
    }
}

/**
 * MatrixInverse calculates the inverse of a matrix and returns the
 * result through the second argument.
 * mat[rows][columns]
 *    c0  c1  c2
 * r0
 * r1
 * r2
 * 
 */
void MatrixInverse(float mat[3][3], float result[3][3])
{
    float minorMat[3][3]; // Create matrix for minor mat.
    float nMat[2][2]; // Holder for 2x2 dets.
    float detMat = MatrixDeterminant(mat); // Find determinant of original matrix.

    // Need to find minor matrix first.
    int i, j;
    for (i = 0; i < 3; ++i) { // Cycle through rows
        for (j = 0; j < 3; ++j) { // Cycle through columns
            if (i == 0) {
                if (j == 0) {
                    nMat[0][0] = mat[1][1];
                    nMat[0][1] = mat[1][2];
                    nMat[1][0] = mat[2][1];
                    nMat[1][1] = mat[2][2];
                }
                if (j == 1) {
                    nMat[0][0] = mat[1][0];
                    nMat[0][1] = mat[1][2];
                    nMat[1][0] = mat[2][0];
                    nMat[1][1] = mat[2][1];
                }
                if (j == 2) {
                    nMat[0][0] = mat[1][0];
                    nMat[0][1] = mat[1][1];
                    nMat[1][0] = mat[2][0];
                    nMat[1][1] = mat[2][1];
                }
            }
            if (i == 1) {
                if (j == 0) {
                    nMat[0][0] = mat[0][1];
                    nMat[0][1] = mat[0][2];
                    nMat[1][0] = mat[2][1];
                    nMat[1][1] = mat[2][2];
                }
                if (j == 1) {
                    nMat[0][0] = mat[0][0];
                    nMat[0][1] = mat[0][2];
                    nMat[1][0] = mat[2][0];
                    nMat[1][1] = mat[2][2];
                }
                if (j == 2) {
                    nMat[0][0] = mat[0][0];
                    nMat[0][1] = mat[0][1];
                    nMat[1][0] = mat[2][0];
                    nMat[1][1] = mat[2][1];
                }
            }
            if (i == 2) {
                if (j == 0) {
                    nMat[0][0] = mat[0][1];
                    nMat[0][1] = mat[0][2];
                    nMat[1][0] = mat[1][1];
                    nMat[1][1] = mat[1][2];
                }
                if (j == 1) {
                    nMat[0][0] = mat[0][0];
                    nMat[0][1] = mat[0][2];
                    nMat[1][0] = mat[1][0];
                    nMat[1][1] = mat[1][2];
                }
                if (j == 2) {
                    nMat[0][0] = mat[0][0];
                    nMat[0][1] = mat[0][1];
                    nMat[1][0] = mat[1][0];
                    nMat[1][1] = mat[1][1];
                }
            }
            minorMat[i][j] = ((nMat[0][0] * nMat[1][1])-(nMat[0][1] * nMat[1][0]));
            ; // Update minorMat to 2x2 det.
        }
    } // End of minorMat Gen. (Matrix of minors)

    // Apply cofactors: minorMat[i][j]
    minorMat[0][1] *= -1.0;
    minorMat[1][0] *= -1.0;
    minorMat[1][2] *= -1.0;
    minorMat[2][1] *= -1.0;

    // Transpose vector:
    float transMex[3][3];
    MatrixTranspose(minorMat, transMex);

    // Multiply by 1/Determinant:
    MatrixScalarMultiply((1.0 / detMat), transMex, result);

}



