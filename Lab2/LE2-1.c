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
//#include<windows.h>

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

void instCtrl(unsigned char data){
	PORTB = data;
	PORTC = 0x02;
	delay(1000);
	PORTC = 0x00;

	return;
}

void dataCtrl(unsigned char data){
	PORTB = data;
	PORTC = 0x03;
	delay(1000);
	PORTC = 0x01;

	return;
}

void initLCD(){
	instCtrl(0x38);
	instCtrl(0x00);
	instCtrl(0x01);
	instCtrl(0x06);
	instCtrl(0x0C);
	
	return;
}

void main()
{	
		TRISB = 0x00; 	// set all bits (port) in PORTB as input
		TRISC = 0x00; 	// set all bits (port) in PORTC as output
		
		initLCD();
		instCtrl(0xC6);
		dataCtrl('H');
		dataCtrl('E');
		dataCtrl('L');
		dataCtrl('L');
		dataCtrl('O');
		dataCtrl('!');

		while(1){
		}
		
	return;
}