/*=================================================================================
	Name: 			Cyril Andre B. Durango
	Date Created: 	February 6, 2025
	Group: 			2 - CPE 3201
	-----------------------------------------
	REVISION HISTORY:
	Date:			By:			Description:

=================================================================================*/

//INCLUDE LINES UNDER HERE
#include <xc.h> // include file for the XC8 compiler

//PRAGMA LINES UNDER HERE
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay(int max){
	int cnt;
	for(cnt=0;cnt<max;cnt++); // delay	

	return;
}

void main()
{	
	unsigned input;
	TRISD = 0x1F;
	TRISA = 0x00;

	if (RD4 == 1){
		input = PORTD & 0x0F;
		switch(input){
			case 0x00: PORTA = 0x01; break;
			case 0x01: PORTA = 0x02; break;
			case 0x02: PORTA = 0x03; break;
			case 0x04: PORTA = 0x04; break;
			case 0x05: PORTA = 0x05; break;
			case 0x06: PORTA = 0x06; break;
			case 0x08: PORTA = 0x07; break;
			case 0x09: PORTA = 0x08; break;
			case 0x0A: PORTA = 0x09; break;
			case 0x0C: PORTA = 0x0F; break;
			case 0x0D: PORTA = 0x00; break;
			case 0x0E: PORTA = 0x0F; break;
		}
		delay(5000);
	}
		
	return;
}