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

int RB0_flag = 0;	//TMR0 flag bit

void init_SPI_Master(){
	TRISC5 = 0; TRISC4 = 1; TRISC3 = 0;
	SSPSTAT = 0x40;	// CKE = 1 (Transmit on active to idle)
	SSPCON = 0x20;	// SPI Master, Fosc/4
}

void SPI_Write(unsigned char data){
	SSPBUF = data;
	while(!SSPIF); // Wait until transmission complete
	SSPIF = 0;     // Clear interrupt flag
}

void initPIC(){
	TRISB = 0x01;	PORTB = 0x00;
	TRISD = 0x0F;	PORTD = 0x00;
	INTEDG = 1;
	INTE = 1; 	INTF = 0;
	GIE = 1;
}

void interrupt ISR(){
	GIE = 0;
	if (INTF){
		INTF = 0;
		RB0_flag = 1;
	}
	GIE = 1;
}

char key_pressed(){
	char key = PORTD & 0x0F;
	switch (key){
		case 0x00: return '1';
		case 0x01: return '2';
		case 0x02: return '3';
		case 0x04: return '4';
		case 0x05: return '5';
		case 0x06: return '6';
		case 0x08: return '7';
		case 0x09: return '8';
		case 0x0A: return '9';
		case 0x0D: return '0';
	}
	return '-';
}

void main(){
	initPIC();
	init_SPI_Master();
	while(1){
		if(RB0_flag){
			RB1 = 1;
			SPI_Write(key_pressed());
			RB0_flag = 0;
			__delay_ms(50); // brief delay after transmission
		}
		RB1 = 0;
	}
}
