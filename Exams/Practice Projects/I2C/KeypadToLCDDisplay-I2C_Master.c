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

//DECLARED GLOBAL VARIABLES HERE (flags)
int RB0_flag = 0;	//TMR0 flag bit

//I2C FUNCTIONS (may or may not be necessary)
void init_I2C_Master(){	// DESC: INITIALIZE MASTER in I2C
	TRISC3 = 1; TRISC4 = 1;	//SCL and SDA as input
	SSPCON = 0x28;	SSPCON2 = 0x00;
	SSPSTAT = 0x00;	SSPADD = 0x09;
}

void I2C_Wait(){	// DESC: INITIALIZE MASTER WAIT
	while((SSPCON2 & 0x1F)||(SSPSTAT & 0x04));
}

void I2C_Start(){	// DESC: INITIALIZE MASTER START
	I2C_Wait();
	SEN = 1;
}

void I2C_RepeatedStart(){	// DESC: INITIALIZE MASTER REPEATED START
	I2C_Wait();
	RSEN = 1;
}

void I2C_Stop(){	// DESC: INITIALIZE MASTER STOP
	I2C_Wait();
	PEN = 1;
}

void I2C_Send(unsigned char data){	// DESC: INITIALIZE MASTER SEND
	I2C_Wait();
	SSPBUF = data;
}

unsigned char I2C_Receive(unsigned char ack){	// DESC: INITIALIZE MASTER RECEIVE
	unsigned char temp;		I2C_Wait();
	RCEN = 1;				I2C_Wait();
	temp = SSPBUF;			I2C_Wait();
	ACKDT = (ack)?0:1;
	ACKEN = 1;
	return temp;
}

void initPIC(){			// DESC: INITIALIZES PIC
	//PORT CONFIGURATIONS (edit and/or remove if necessary)
	TRISB = 0x01;	PORTB = 0x00;	//PORTB config
	TRISD = 0xFF;	PORTD = 0x00;	//PORTB config
	
	//PERIPHERAL CONFIGURATIONS (edit and/or remove if necessary)
	INTEDG = 1; 
	
	//INTERRUPT CONFIGURATIONS (remove if necessary)
	INTE = 1; 	INTF = 0;		//TMR0 Interrupt
	GIE = 1;					//GLOBAL INTERRUPT
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

void main(){			// DESC: MAIN FUNCTION
	initPIC();
	init_I2C_Master();
	//foreground routine
	unsigned char temp = 0;
	for(;;){
		if(RB0_flag){
			I2C_Start();
			I2C_Send(0x10);
			I2C_Send(key_pressed());
			I2C_Stop();
			__delay_ms(1); 				// delay
			RB0_flag = 0;
		}
	}
}


