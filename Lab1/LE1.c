/*=================================================================================
	Name: 			Vince Louie Q. Valle
	Date Created: 	February 5, 2025
	Group: 			2 - CPE 3201
	-----------------------------------------
	REVISION HISTORY:
	Date:			By:			Description:
	02-05-25		CABD		Optimized code and added necessary documentation
=================================================================================*/

//INCLUDE LINES UNDER HERE
#include <xc.h> // include file for the XC8 compiler

//PRAGMA LINES UNDER HERE
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma confiag BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void main()
{	
		int i, cnt;
		TRISB = 0x00; 	// set all bits (port) in PORTB as output
		TRISA = 0x01; 	// set all bits (port) in PORTA as input
		ADCON1 = 0x06; 	// convert push button analog to digital
		
		PORTB = 0x00;
		if(RA0 == 1){
			i = 0;
			for(i=0; i<3; i++)
				{
					for(cnt=0;cnt<10000;cnt++); // delay
					PORTB = 0x01; // set RB0 to 1 (LED ON)
					for(cnt=0;cnt<10000;cnt++); // delay
					PORTB = 0x00; // set RB0 to 0 (LED OFF)
					
				}
		}
	return;
}