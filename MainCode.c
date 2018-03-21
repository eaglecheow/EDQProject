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
#include "p18f46k20.h"
#include "delays.h"
#include "04 Switch Input.h"  // header file
#include "12 CCP PWM.h"
#include <string.h>

/** V A R I A B L E S *************************************************/
#pragma udata   // declare statically allocated uinitialized variables

/** D E C L A R A T I O N S *******************************************/
#pragma code    // declare executable instructions
#define Switch_Pin2	PORTBbits.RB1
#define Switch_Pin3 PORTBbits.RB3
#define Switch_Pin4 PORTBbits.RB2

int Check_Motor_Direc(int motor_direc_value);
int Check_on_or_Off (int Current_Power);
int Check_Speed(int CurrentPower);
void SetPowerLevel(int powerLevel);
int IncreasePower(int currentPower);
int DecreasePower(int currentPower);
void InitializeSystem(void);
void PrintToScreen(char* message);


void main (void)
{
    //Initialize System and wait for 30000 until init finish
    InitializeSystem();
    delay = 30000;
	while(delay--);

	int power = 0;
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
	//TRISBbits.TRISB0 = 1;
    //TRISBbits.TRISB1 = 1;       // PORTB bit 0 (connected to switch) is input (1)
	
    while (1){
		power = Check_on_or_Off(power);
		//Delay10TCYx(1000);
		motor_direc = Check_Motor_Direc(motor_direc);
		//power = Check_on_or_Off(power);
		
		power = Check_Speed(power);
		
	}
}
	
	int Check_Speed(int CurrentPower)
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
				return CurrentPower;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin != 1);
		
		return IncreasePower(CurrentPower);
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
				return CurrentPower;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin2 != 1);

	   return DecreasePower(CurrentPower);
	}
	
	else {
		return CurrentPower;
	}	
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
	if (currentPower > 1)
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

int Check_on_or_Off (int Current_Power){

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
				return Current_Power;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow);
       	while (Switch_Pin3 != 1);
			SetPowerLevel(0);
			return 0;
		//if(Current_Power == 0){
			//SetPowerLevel(1);
			//return 1; 
			//}
	//	else {
			//SetPowerLevel(0);
		//	return 0;
			//}
	}
}

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
			return 1; 
			}
		else {
			CCP1CON = 0b11001100;
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

	TRISB = 0b11001111;


	LATC = 0b00101000;
	TRISC = 0b00000000;

	TRISD = 0;								// TRISD is LED output
	LATD = 0;								// turn off LEDS

	TRISE = 0b00000111;

	//configure buttons
	WPUB = 0b00001111;
	INTCON2bits.RBPU = 0; 					// turn on weak pull ups for RB0-RB3

	INTCONbits.INT0IF = 0;					// clear RB0 INT
	INTCONbits.INT0IE = 1;					// enable RB0 INT
	INTCON2bits.INTEDG0 = 0;				// interrupt on falling edge 

	INTCON3bits.INT1IF = 0;					// clear RB1 INT
	INTCON3bits.INT1IE = 1;					// enable RB1 INT	
	INTCON2bits.INTEDG1 = 0;				// interrupt on falling edge

	INTCON3bits.INT2IF = 0;					// clear RB2 INT
	INTCON3bits.INT2IE = 1;					// enable RB2 INT
	INTCON2bits.INTEDG2 = 0;				// interrupt on falling edge

	
	// Setup analog functionality
	ANSEL = 0x00;							// all pins digital
	ANSELH = 0x00;
	
	ANSELbits.ANS6=1;						// RE2 is a temperature input


	ADCON1=0;								// Reference Vdd and Vss
	ADCON2=0b10001100;						// right, AN6, 2 Tad, Fosc/64
	ADCON0=0b00011101;						// turn on ADC


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

    oled_init();

} // end InitializeSystem

/**
 * Prints the message to the OLED screen
 * @param message Message to display
 */ 
void PrintToScreen(char* message)
{
    oled_clear();
    oled_refresh();
    for (int i = 0; i < strlen(message); i++)
    {
        oled_putc_2x(message[i]);
    }
}
