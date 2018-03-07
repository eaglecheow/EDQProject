/*
 * File:   MainCode.c
 * Author: kecy6cyt
 *
 * Created on March 7, 2018, 2:09 PM
 */

//Configuration
#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                       // CONFIG1H
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 30                        // CONFIG2L
#pragma config WDTEN = OFF, WDTPS = 32768                                     // CONFIG2H
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC      // CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF                          // CONFIG4L
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                   // CONFIG5L
#pragma config CPB = OFF, CPD = OFF                                         // CONFIG5H
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF               // CONFIG6L
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF                           // CONFIG6H
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           // CONFIG7L
#pragma config EBTRB = OFF

#include <xc.h>
#include "p18f46k20.h"
#include "12 CCP PWM.h"
#include <string.h>

enum PowerLevel
{
    Off, Low, MediumLow, Medium, MediumHigh, High
};

void SetPowerLevel(PowerLevel powerLevel);
void PrintToScreen(char lineOneMessage[], char lineTwoMessage[]);

unsigned char power = 0;

void main(void) 
{
    
    // Setup RD7/P1D as output
    TRISDbits.TRISD7 = 0;
    
    //Scale timer to 1:16
    T2CON = 0b00000111;
    PR2 = 249;
    CCPR1L = 0x7D;
    CCP1CON = 0b01001100;
    PowerLevel powerLevel = PowerLevel.Low;
    
    //Prevents main function from accessing invalid memory
    while(1);
}

/**
 * This function sets the power value depends on the PowerLevel enum
 * @param powerLevel Power level to set
 */
void SetPowerLevel(PowerLevel powerLevel)
{
    switch (powerLevel)
    {
        case PowerLevel.Off:
        {
            power = 0;
            break;
        }
        case PowerLevel.Low:
        {
            power = 50;
            break;
        }
        case PowerLevel.MediumLow:
        {
            power = 100;
            break;
        }
        case PowerLevel.Medium:
        {
            power = 150;
            break;
        }
        case PowerLevel.MediumHigh:
        {
            power = 200;
            break;
        }
        case PowerLevel.High:
        {
            power = 250;
            break;
        }
        default:
        {
            power = 0;
            break;
        }
        
        //Sets the value to the PICKIT
        CCPR1L = power;
    }
}

/**
 * This function prints the message to the OLED screen on PICKIT
 * @param lineOneMessage First line of the message
 * @param lineTwoMessage Second line of the message
 */
void PrintToScreen(char lineOneMessage[], char lineTwoMessage[])
{
    int counter = 0;
    for (counter = 0; counter < strlen(lineOneMessage); counter++)
    {
        //TODO: Put print character function
    }
    //TODO: Print linebreak character
    for (counter = 0; counter < strlen(lineTwoMessage); counter++)
    {
        //TODO: Put print character function
    }
}
