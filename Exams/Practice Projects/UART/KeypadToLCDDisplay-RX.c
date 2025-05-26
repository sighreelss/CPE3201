// TITLE: 	MPLAB IDE PIC16F977A CODE TEMPLATE
// CREATOR:	CYRIL ANDRE DURANGO

// INCLUDE LINES UNDER HERE
#include <xc.h>
#include <math.h>

// PRAGMA LINES UNDER HERE
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF 	
#pragma config CP = OFF

//DEFINE LINES UNDER HERE (#define <DATA_NAME> data)

//DECLARED GLOBAL VARIABLES HERE (flags)
#define _XTAL_FREQ 4000000

//LCD FUNCTIONS (may or may not be necessary)
void LCD_instCtrl(unsigned char inst){	// DESC: CONTROLS LCD INSTRUCTION
	PORTB = inst;
	RC0 = 0;
	RC1 = 1;
	__delay_ms(1);
	RC1 = 0;
}	

void LCD_dataCtrl(unsigned char data){	// DESC: CONTROLS LCD DATA DISPLAY
	PORTB = data;
	RC0 = 1;
	RC1 = 1;
	__delay_ms(1);
	RC1 = 0;
}

void initLCD(){			// DESC: INITIALIZES LCD
	LCD_instCtrl(0x38);
	LCD_instCtrl(0x0C);
	LCD_instCtrl(0x06);
	LCD_instCtrl(0x01);
	return;
}

void initPIC(){			// DESC: INITIALIZES PIC
	//PORT CONFIGURATIONS (edit and/or remove if necessary)
	TRISB = 0x00;	PORTB = 0x00;	//PORTB config
	TRISC0 = 0;	
	TRISC1 = 0;
	TRISC7 = 1;
	TRISD = 0x00;	PORTD = 0x00;
	
	//PERIPHERAL CONFIGURATIONS (edit and/or remove if necessary)

	SPBRG = 0x19;	SYNC = 0;	BRGH = 1;	SPEN = 1; //USART CONFIGURATIONS
	RX9 = 0;	CREN = 1;	//USART ASYNCH RECEIVE
	
	//INTERRUPT CONFIGURATIONS (remove if necessary)
	GIE = 1;	//GLOBAL INTERRUPT
	

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
	}
	PORTD = 0b01000000; return;
}

void main(){			// DESC: MAIN FUNCTION
	initPIC();
	initLCD();
	
	//foreground routine
	while(1){
		while(!RCIF);
		unsigned char temp = RCREG;
		LCD_instCtrl(0x80);
		LCD_dataCtrl(temp);
		sevenSegDisp(temp);
	}
}




