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
#define _XTAL_FREQ 4000000

unsigned char temp = 0;
unsigned char data_r = 0;
unsigned char data_w = 0;

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

void init_I2C_Slave(unsigned char slave_add){	// DESC: INITIALIZE SLAVE in I2C
	TRISC3 = 1; TRISC4 = 1;	//SCL and SDA as input
	SSPCON = 0x36;	SSPCON2 - 0x01;
	SSPSTAT = 0x80;	SSPADD = slave_add;
	SSPIE = 1; SSPIF = 0;	// slave interrupt
	PEIE = 1;	GIE = 1;	// enable interrupt
}

//NECESSARY FUNCTIONS
void delay(int cnt){	// DESC: SETS PIC DELAY
	while(cnt--);
}

void initPIC(){			// DESC: INITIALIZES PIC
	//PORT CONFIGURATIONS (edit and/or remove if necessary)
	TRISB = 0x00;	PORTB = 0x00;	//PORTB config
	TRISC = 0x00;	PORTC = 0x00;	//PORTC config
	TRISD = 0x00;	PORTD = 0x00;	//PORTD config
}

void interrupt ISR(){	// DESC: interrupt module for I2C - edit/remove if needed 
	CKP = 0; 			// hold clock low (SSPCON reg)
	if(WCOL || SSPOV) 	// check if overflow or data collision (SSPCON reg)
	{
		temp = SSPBUF; 	// read SSPBUF to clear buffer
		WCOL = 0; 		// clear data collision flag
		SSPOV = 0; 		// clear overflow flag
		CKP = 1; 		// release clock (SSPCON reg)
	}
	
	if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) // write to slave
	{
		temp = SSPBUF; // read SSPBUF to clear buffer
		while(!BF); // wait until receive is complete (SSPSTAT reg)
		
		data_r = SSPBUF;	// read data from SSPBUF
		
		CKP = 1; // release clock (SSPCON reg)
	}
	else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) // read from slave
	{
		temp = SSPBUF; // read SSPBUF to clear buffer
		BF = 0; // clear buffer status bit (SSPSTAT reg)
		SSPBUF = data_w;	// send data by writing to SSPBUF
		CKP = 1; // release clock (SSPCON reg)
		while(BF); // wait until transmit is complete (SSPSTAT reg)
	}
	SSPIF = 0; // clear interrupt flag
	
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
	PORTD = 0b00000000; return;
}

void main(){			// DESC: MAIN FUNCTION
	initPIC();
	init_I2C_Slave(0x10);
	initLCD();
	
	//foreground routine
	while(1){
		LCD_instCtrl(0x80);
		LCD_dataCtrl(data_r);
		sevenSegDisp(data_r);
	}
}

