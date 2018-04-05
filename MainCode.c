#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                       // CONFIG1H
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 30                        // CONFIG2L
#pragma config WDTEN = OFF, WDTPS = 32768                                     // CONFIG2H
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = ON, CCP2MX = PORTC       // CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF                          // CONFIG4L
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                   // CONFIG5L
#pragma config CPB = OFF, CPD = OFF                                         // CONFIG5H
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF               // CONFIG6L
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF                           // CONFIG6H
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           // CONFIG7L
#pragma config EBTRB = OFF                                                  // CONFIG7H


/** I N C L U D E S **************************************************/
#include <p18f46k20.h>
#include "delays.h"
#include "04 Switch Input.h"  // header file
//#include "12 CCP PWM.h"
#include <string.h>
#include "oled.h"
#include "oled_interface.h"

/** V A R I A B L E S *************************************************/
#pragma udata   // declare statically allocated uinitialized variables

/** D E C L A R A T I O N S *******************************************/
#pragma code    // declare executable instructions
#define Switch_Pin2	PORTBbits.RB1
#define Switch_Pin3 PORTBbits.RB3
#define Switch_Pin4 PORTBbits.RB2

int Check_Motor_Direc(int motor_direc_value);
void Check_on_or_Off ();
void Check_Speed();
void SetPowerLevel(int powerLevel);
void IncreasePower();
void DecreasePower();
void PrintToScreen();
char NumberToChar(int number);
unsigned int ReadTemperature();
void InitializeADC();

void InitializeSystem(void);

int temperature = 0;
//int speed = 0;
int power = 0;

//Constants
const int WarningTemperature = 50;



void main (void)
{
	int motor_direc = 0; //CW rotation

    TRISD = 0b00000000;     	// PORTD bits 7:0 are all outputs (0)
	T2CON = 0b00000111;
	PR2 = 249;
	CCPR1L = 0;
	CCP1CON = 0b11001100;

	INTCON2bits.RBPU = 0;		// enable PORTB internal pullups
	WPUBbits.WPUB1 = 1;	
    WPUBbits.WPUB0 = 1;		// enable pull up on RB0
    ANSELH = 0x00;              // AN8-12 are digital inputs (AN12 on RB0)

	InitializeADC();

	InitializeSystem();

	Delay10TCYx(3000);

	oled_init();

	PrintToScreen();
    while (1){
		Check_on_or_Off();
		motor_direc = Check_Motor_Direc(motor_direc);
		if (power> 0)
		{
			Check_Speed();	
		}
        temperature = ReadTemperature();
        if (temperature > WarningTemperature)
        {
            LATDbits.LATD7 = 1;
        }
        else 
        {
            LATDbits.LATD7 = 0;
        }
	}
}

/**
 * Checks for button input, changes the speed of the motor
 */	
void Check_Speed()
{
	int Switch_Count = 0;
	if ((Switch_Pin == 0) && (Switch_Pin2 != 0)){
		do
        { // monitor switch input for 5 lows in a row to debounce
            if ((Switch_Pin == 0))
            { // pressed state detected
                Switch_Count++;
            }
            else
            {
                Switch_Count = 0;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin != 1);
		IncreasePower();
	}
	
	else if ((Switch_Pin2 == 0) && (Switch_Pin != 0)){
		do
        { // monitor switch input for 5 lows in a row to debounce
            if ((Switch_Pin2 == 0))
            { // pressed state detected
                Switch_Count++;
            }
            else
            {
                Switch_Count = 0;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin2 != 1);

	   DecreasePower();
	}
}

/**
 * Increases power level by one
 */
void IncreasePower()
{
	if (power < 5)
	{
		SetPowerLevel(++power);
	}
	temperature = ReadTemperature();
}

/**
 * Decrease power level by one
 */
void DecreasePower()
{
	if (power > 1)
	{
		SetPowerLevel(--power);
	}
	temperature = ReadTemperature();
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
			power = 0;
            CCPR1L = 0;
            break;
        }
        case 1:
        {
			power = 1;
            CCPR1L = 50;
            break;
        }
        case 2:
        {
			power = 2;
            CCPR1L = 100;
            break;
        }
        case 3:
        {
			power = 3;
            CCPR1L = 150;
            break;
        }
        case 4:
        {
			power = 4;
            CCPR1L = 200;
            break;
        }
        case 5:
        {
			power = 5;
            CCPR1L = 250;
            break;
        }
        default:
        {
			power = 0;
            CCPR1L = 0;
            break;
        }
    }
	PrintToScreen();
}

/**
 * Checks the button input, turns on/off the motor
 */
void Check_on_or_Off (){

		int Switch_Count = 0;
	if (Switch_Pin3 == 0){
		do
        { // monitor switch input for 5 lows in a row to debounce
            if ((Switch_Pin3 == 0))
            { // pressed state detected
                Switch_Count++;
            }
            else
            {
                Switch_Count = 0;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin3 != 1);
		if(power == 0){
			SetPowerLevel(1); 
			}
		else {
			SetPowerLevel(0);
			}
	}
}

/**
 * Checks for button input and change the motor direction
 * @param motor_direc_value Current direction of the motor
 * @return New direction of the motor
 */
int Check_Motor_Direc(int motor_direc_value){
		int Switch_Count = 0;
	if (Switch_Pin4 == 0){
		do
        { // monitor switch input for 5 lows in a row to debounce
            if ((Switch_Pin4 == 0))
            { // pressed state detected
                Switch_Count++;
            }
            else
            {
                Switch_Count = 0;
				return motor_direc_value;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin4 != 1);
		if(motor_direc_value == 0){
			CCP1CON = 0b01001100;
			PrintToScreen();
			return 1; 
			}
		else {
			CCP1CON = 0b11001100;
			PrintToScreen();
			return 0;
			}
	}
	return motor_direc_value;
}

/**
 * Initializes the OLED
 */
void InitializeSystem(void)
{

	OSCCON = 0b01110000;
	OSCTUNEbits.PLLEN = 0; 					// turn off the PLL

	// Setup I/O Ports.
	
	TRISA = 0;								// TRISA outputs
	LATA = 0b11110000;						// drive PORTA pins low

	oled_res = 1;							// do not reset LCD yet
	oled_cs = 1;							// do not select the LCD


	LATC = 0b00101000;
	TRISC = 0b00000000;

	TRISD = 0;								// TRISD is LED output
	LATD = 0;								// turn off LEDS

	TRISE = 0b00000111;


	// Setup TMR1
	// Configure Timer 1
	T1CON 	= 0b00001111;

		
	// Setup TMR2
	T2CON = 0b00000100;						// 1:1 prescaler
	PR2 = 0xFF;
	T0CON = 0x80;

	// Configure MSSP for SPI master mode
	SSPCON1 = 0b00110010;					// clock idle high, Clk /64

	SSPSTAT = 0b00000000;

	PIR1bits.TMR1IF = 0; 
	PIE1bits.TMR1IE = 1; 

	INTCONbits.PEIE = 1; 
	INTCONbits.GIE = 0; 

    //oled_init();

} // end InitializeSystem

/**
 * Prints the message to the OLED screen
 * @param message Message to display
 */ 
void PrintToScreen()
{
	temperature = ReadTemperature();
	
	//oled_init();
	oled_clear();
	oled_refresh();

	oled_putc_2x('S');
	oled_putc_2x('t');
	oled_putc_2x('a');
	oled_putc_2x('t');
	oled_putc_2x('u');
	oled_putc_2x('s');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x(':');
	oled_putc_2x(' ');
	oled_putc_2x('O');
	oled_putc_2x((power == 0)?'F':'N');
	oled_putc_2x((power == 0)?'F':' ');

	oled_putc_2x('\n');

	oled_putc_2x('T');
	oled_putc_2x('e');
	oled_putc_2x('m');
	oled_putc_2x('p');
	oled_putc_2x('.');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x(' ');
	oled_putc_2x(':');
	oled_putc_2x(' ');
	oled_putc_2x(NumberToChar(temperature/100));
	oled_putc_2x(NumberToChar((temperature/10)-(temperature/100)*10));
	oled_putc_2x(NumberToChar(temperature%10));

	oled_putc_2x('\n');

	oled_putc_2x('S');
	oled_putc_2x('p');
	oled_putc_2x('e');
	oled_putc_2x('e');
	oled_putc_2x('d');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x(' ');
	oled_putc_2x(':');
	oled_putc_2x(' ');
	//oled_putc_2x(NumberToChar(speed/10));
	//oled_putc_2x(NumberToChar(speed%10));
	oled_putc_2x('0');
	oled_putc_2x(NumberToChar(power));

	oled_putc_2x('\n');

	oled_putc_2x('D');
	oled_putc_2x('i');
	oled_putc_2x('r');
	oled_putc_2x('e');
	oled_putc_2x('c');
	oled_putc_2x('t');
	oled_putc_2x('i');
	oled_putc_2x('o');
	oled_putc_2x('n');
	oled_putc_2x('\t');
	oled_putc_2x('\t');
	oled_putc_2x(':');
	oled_putc_2x(' ');
	oled_putc_2x((CCP1CON == 0b01001100)?'L':'R');



	oled_refresh();
}

/**
 * Converts integer to it's respective char
 * @param number Number in integer form
 */
char NumberToChar(int number)
{
	char c = '/0';
	switch (number)
	{
		case 0:
		c = '0';
		break;	
		case 1:
		c = '1';
		break;	
		case 2:
		c = '2';
		break;	
		case 3:
		c = '3';
		break;	
		case 4:
		c = '4';
		break;	
		case 5:
		c = '5';
		break;	
		case 6:
		c = '6';
		break;	
		case 7:
		c = '7';
		break;	
		case 8:
		c = '8';
		break;	
		case 9:
		c = '9';
		break;		
	}
	return c;
}

/**
 * Initialize the ADC system
 */
void InitializeADC()
{
    //TODO: Check to see if any parameters can be changed
	ANSEL = 0;
	ANSELH = 0;
	ANSELbits.ANS5 = 1;
	ADCON2 = 0b00111000;
	ADCON0 = 0b00010101;
}

/**
 * Reads the temperature from the device, also the ADC
 */
unsigned int ReadTemperature()
{
	ADCON0bits.GO_DONE = 1;
	while (ADCON0bits.GO_DONE == 1);

	return (int)ADRESH;
}
