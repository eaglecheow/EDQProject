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
#include "p18f46k20.h"
#include "12 CCP PWM.h"
#include <string.h>


void SetPowerLevel(int powerLevel);
void PrintToScreen(char lineOneMessage[], char lineTwoMessage[]);
int IncreasePower(int currentPower);
int DecreasePower(int currentPower);

void main(void) 
{
    int power = 5;
    // Setup RD7/P1D as output
    TRISDbits.TRISD7 = 0;
    
    //Scale timer to 1:16
    T2CON = 0b00000111;
    PR2 = 249;
    SetPowerLevel(power);
    CCP1CON = 0b01001100;
	power = DecreasePower(power);
	power = DecreasePower(power);
	//Delay1KTCYx(100);
	power = DecreasePower(power);
	



    
    //Prevents main function from accessing invalid memory
    while(1);
}

/**
 * Increases power level by one
 */
int IncreasePower(int currentPower)
{
	if (currentPower < 5)
	{
		SetPowerLevel(++currentPower);
	}
	return currentPower;
}

/**
 * Decrease power level by one
 */
int DecreasePower(int currentPower)
{
	if (currentPower > 0)
	{
		SetPowerLevel(--currentPower);
	}
	return currentPower;
}

/**
 * This function sets the power value depends on the PowerLevel enum
 * @param powerLevel Power level to set
 */
void SetPowerLevel(int powerLevel)
{
    switch (powerLevel)
    {
        case 0:
        {
            CCPR1L = 0;
            break;
        }
        case 1:
        {
            CCPR1L = 50;
            break;
        }
        case 2:
        {
            CCPR1L = 100;
            break;
        }
        case 3:
        {
            CCPR1L = 150;
            break;
        }
        case 4:
        {
            CCPR1L = 200;
            break;
        }
        case 5:
        {
            CCPR1L = 250;
            break;
        }
        default:
        {
            CCPR1L = 0;
            break;
        }
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
