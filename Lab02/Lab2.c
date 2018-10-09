// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
double Round(double operand);
double multiply(double a, double b);
double divide(double a, double b);
double add(double a, double b);
double subtract(double a, double b);
double AbsoluteValue(double absIN);
double Average(double a, double b);
double FahrenheitToCelsius(double fahr);
double CelsiusToFahrenheit(double celsius);
double Tangent(double input);
void newln();

// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void)
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB2_TESTING

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    
    // Delay to stop errors in the serial interface when connecting to the uC32.
    int i = 0;
    for (i = 0; i < 9999999; i++) {
        // Do nothing.
    }
    
    // Say Hello and tell the user what this is.
    printf("Hello! Welcome to Kyle O'Rourke's Calculator.");
    while (1) {
        // Let user know what to do and gather input.
        char inputChar, charBuff;

        printf("\nEnter a math operation to perform (*,/,+,-,v,a,c,f,t): ");
        scanf("%c", &inputChar, &charBuff);
        printf("\n");

        // Input doubles
        double inputONE;
        double inputTWO;

        // Check input char for each possible input:
        if (inputChar == '*') {          // Multiply
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            printf("Enter second operand: ");
            scanf("%f%c", &inputTWO, &charBuff);
            // Send inputs to multiply function
            double result = multiply(inputONE, inputTWO);
            // Print the result.
            printf("Result of (%f * %f): %f\n", inputONE, inputTWO, result);

        } else if (inputChar == '/') {          // Divide
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            printf("Enter second operand: ");
            scanf("%f%c", &inputTWO, &charBuff);
            // Send inputs to function
            double result = divide(inputONE, inputTWO);
            // Print the result.
            printf("Result of (%f / %f): %f\n", inputONE, inputTWO, result);

        } else if (inputChar == '+') {          // Addition
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            printf("Enter second operand: ");
            scanf("%f%c", &inputTWO, &charBuff);
            // Send inputs to function
            double result = add(inputONE, inputTWO);
            // Print the result.
            printf("Result of (%f + %f): %f\n", inputONE, inputTWO, result);

        } else if (inputChar == '-') {          // Subtraction
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            printf("Enter second operand: ");
            scanf("%f%c", &inputTWO, &charBuff);
            // Send inputs to function
            double result = subtract(inputONE, inputTWO);
            // Print the result.
            printf("Result of (%f - %f): %f\n", inputONE, inputTWO, result);

        } else if (inputChar == 'v') {          // Average
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            printf("Enter second operand: ");
            scanf("%f%c", &inputTWO, &charBuff);
            // Send inputs to function
            double result = Average(inputONE, inputTWO);
            // Print the result.
            printf("Result of avg(%f, %f): %f\n", inputONE, inputTWO, result);

        } else if (inputChar == 'a') {          // ABS value
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            // Send inputs to function
            double result = AbsoluteValue(inputONE);
            // Print the result.
            printf("Result of abs(%f): %f\n", inputONE, result);

        } else if (inputChar == 'c') {   // Converts Fahrenheit to degrees Celsius.
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            // Send inputs to function
            double result = FahrenheitToCelsius(inputONE);
            // Print the result.
            printf("Result of F(%f) to C: %f\n", inputONE, result);

        } else if (inputChar == 'f') {  // Converts Celsius to degrees Fahrenheit.
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            // Send inputs to function
            double result = CelsiusToFahrenheit(inputONE);
            // Print the result.
            printf("Result of C(%f) to F: %f\n", inputONE, result);

        } else if (inputChar == 't') {          // tangent function (Last lab). 
            printf("Enter first operand: ");
            scanf("%f%c", &inputONE, &charBuff);
            // Send inputs to function
            double result = Tangent(inputONE);
            // Print the result.
            printf("Result of tan(%f): %f\n", inputONE, result);
        } else {
            printf(" Input not recognized!\n");
        }

    }


    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/
    while (1);
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double absIN)
{
    if (absIN < 0) {
        absIN = -1 * absIN;
    }
    return (absIN);
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double fahr)
{
    return ((5.0 / 9.0)*(fahr - 32.0));
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double celsius)
{
    return ((celsius * 1.8) + 32);
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double a, double b)
{
    return ((a + b) / 2);
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
double Tangent(double input)
{
    // Degrees to Rad: (angle*pi)/180
    input = (input * M_PI) / 180.0;
    input = tan(input);
    input = (input * 180.0) / M_PI;
    return input;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    return operand;
}

// Multiply function

double multiply(double a, double b)
{
    return (a * b);
}

// Divide function

double divide(double a, double b)
{
    if (b == 0) {
        return 0; // Divide by zero catch
    }
    return (a / b);
}

// add function

double add(double a, double b)
{
    return (a + b);
}

// subtract function

double subtract(double a, double b)
{
    return (a - b);
}


// Newline Function

void newln()
{
    printf("\n");
}