// TITLE: 	MPLAB IDE PIC16F977A CODE TEMPLATE
// CREATOR:	CYRIL ANDRE DURANGO

#include <xc.h>
#include <math.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF 	
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

unsigned char data_r = 0;

void LCD_instCtrl(unsigned char inst){
	PORTB = inst;
	RC0 = 0;
	RC1 = 1;
	__delay_ms(10);
	RC1 = 0;
}	

void LCD_dataCtrl(unsigned char data){
	PORTB = data;
	RC0 = 1;
	RC1 = 1;
	__delay_ms(10);
	RC1 = 0;
}

void initLCD(){
	LCD_instCtrl(0x38);
	LCD_instCtrl(0x0C);
	LCD_instCtrl(0x06);
	LCD_instCtrl(0x01);
}

void init_SPI_Slave(){
	TRISC5 = 0; TRISC4 = 1; TRISC3 = 1;
	TRISA5 = 1;
	SSPSTAT = 0x40;	// CKE = 1 (Data transmitted on rising edge)
	SSPCON = 0x24;	// SPI Slave mode with SS pin enabled
	SSPIE = 1;
	PEIE = 1;
	GIE = 1;
}

void interrupt ISR(){
	if (SSPIF){
		SSPIF = 0;
		data_r = SSPBUF;
	}
}

void sevenSegDisp(unsigned char key){
	switch (key){
		case '1': PORTD = 0b00000110; return;
		case '2': PORTD = 0b01011011; return;
		case '3': PORTD = 0b01001111; return;
		case '4': PORTD = 0b01100110; return;
		case '5': PORTD = 0b01101101; return;
		case '6': PORTD = 0b01111101; return;
		case '7': PORTD = 0b00000111; return;
		case '8': PORTD = 0b01111111; return;
		case '9': PORTD = 0b01101111; return;
		case '0': PORTD = 0b00111111; return;
		case '-': PORTD = 0b01000000; return;
	}
	PORTD = 0b00000000;
}

void initPIC(){
	TRISB = 0x00;	PORTB = 0x00;
	TRISC0 = 0;	TRISC1 = 0;
	TRISD = 0x00;	PORTD = 0x00;
	ADCON1 = 0x06;
}

void main(){
	initPIC();
	initLCD();
	init_SPI_Slave();
	
	while(1){
		if (data_r != 0){
			LCD_instCtrl(0x80);
			LCD_dataCtrl(data_r);
			sevenSegDisp(data_r);
			data_r = 0;
			__delay_ms(200);
		}
	}
}
