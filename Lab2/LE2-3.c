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
	delay(5000);
	PORTC = 0x00;

	return;
}

void dataCtrl(unsigned char data){
	PORTB = data;
	PORTC = 0x03;
	delay(5000);
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

void main(){	
		unsigned char input;
		unsigned char curr_LCD_pos = 0x80;
		unsigned char max_LCD_pos = 0xE8;
		TRISB = 0x00; 	// set all bits (port) in PORTB as outputs	- data signals for LCD
		TRISC = 0x00; 	// set all bits (port) in PORTC as outputs	- control signals for LCD
		TRISD = 0xFF;	// set all bits (port) in PORTD as inputs	- decoded input from keypad		

		initLCD();
		while (curr_LCD_pos <= max_LCD_pos){
			instCtrl(curr_LCD_pos);
			if(RD4 == 1){
				input = PORTD & 0x0F;
				switch(input){	
					case 0x00: dataCtrl('1'); break;
					case 0x01: dataCtrl('2'); break;
					case 0x02: dataCtrl('3'); break;
					case 0x04: dataCtrl('4'); break;
					case 0x05: dataCtrl('5'); break;
					case 0x06: dataCtrl('6'); break;
					case 0x08: dataCtrl('7'); break;
					case 0x09: dataCtrl('8'); break;
					case 0x0A: dataCtrl('9'); break;
					case 0x0C: dataCtrl('*'); break;
					case 0x0D: dataCtrl('0'); break;
					case 0x0E: dataCtrl('#'); break;
				}
				
				delay(5000);
				if(curr_LCD_pos == 0x93){
					curr_LCD_pos = 0xBF;
				} else if (curr_LCD_pos == 0xD3){
					curr_LCD_pos = 0x93;
				} else if (curr_LCD_pos == 0xA7){
					curr_LCD_pos = 0xD3;
				} else if (curr_LCD_pos == max_LCD_pos){
					instCtrl(0x01);
					curr_LCD_pos = 0x7F;
				}
				curr_LCD_pos++;
			} 
		}	
	return;
}
