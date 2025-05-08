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
int RB0_flag = 0	//TMR0 flag bit
int ADC_flag = 0	//CCP1 flag bit

//LCD FUNCTIONS (may or may not be necessary)
void LCD_instCtrl(){	// DESC: CONTROLS LCD INSTRUCTION
	
}

void LCD_dataCtrl(){	// DESC: CONTROLS LCD DATA DISPLAY
	
}

void initLCD(){			// DESC: INITIALIZES LCD
	
}

//I2C FUNCTIONS (may or may not be necessary)
void init_I2C_Master(){	// DESC: INITIALIZE MASTER in I2C
	TRISC3 = 1; TRISC4 = 1;	//SCL and SDA as input
	SSPCON = 0x28;	SSPCON2 - 0x00;
	SSPSTAT = 0x00;	SSPADD = 0x09;
}

void init_I2C_Slave(unsigned char slave_add){	// DESC: INITIALIZE SLAVE in I2C
	TRISC3 = 1; TRISC4 = 1;	//SCL and SDA as input
	SSPCON = 0x36;	SSPCON2 - 0x01;
	SSPSTAT = 0x80;	SSPADD = slave_add;
	SSPIE = 1; SSPIF = 0;	// slave interrupt
	PEIE = 1;	GIE = 1;	// enable interrupt
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
	SSPBUF = data;
}

unsigned char I2C_Receive(unsigned char ack){	// DESC: INITIALIZE MASTER RECEIVE
	unsigned char temp;	I2C_Wait();
	RCEN = 1;	I2C_Wait();
	temp = SSPBUF;	I2C_Wait();
	ACKDT = (ack)?0:1;
	ACKEN = 1;
	return temp;
}

//NECESSARY FUNCTIONS
void delay(int cnt){	// DESC: SETS PIC DELAY
	while(cnt--);
}

void initPIC(){			// DESC: INITIALIZES PIC
	//PORT CONFIGURATIONS (edit and/or remove if necessary)
	TRISA = 0x00;	PORTA = 0x00;	//PORTA config
	TRISB = 0x00;	PORTB = 0x00;	//PORTB config
	TRISC = 0x00;	PORTC = 0x00;	//PORTC config
	TRISD = 0x00;	PORTD = 0x00;	//PORTD config
	
	//PERIPHERAL CONFIGURATIONS (edit and/or remove if necessary)
	INTEDG = 1; 	OPTION_REG = 0x00;			//TIMER0 CONFIG
	T1CON = 0x00;	TMR1 = 0x0000;	TMR1ON = 0;	//TIMER1 CONFIG
	T2CON = 0x00;	PR2 = 0x00;		TMR2ON = 0;	//TIMER1 CONFIG
	ADCON1 = 0x00;	ADCON0 = 0x00; 				//ADC CONFIG
	CCP1CON = 0x00;	CCPR1CON = 0x00;			//CAPTURE, COMPARE, PWM CONFIG for CCP1
	CCP2CON = 0x00;	CCPR2CON = 0x00;			//CAPTURE, COMPARE, PWM CONFIG for CCP2
	
	SPBRG = 0x19;	SYNC = 0;	BRGH = 1;	SPEN = 1; //USART CONFIGURATIONS
	TX9 = 0;	TXEN = 0;	//USART ASYNCH TRANSMIT
	RX9 = 0;	CREN = 0;	//USART ASYNCH RECEIVE
	
	//INTERRUPT CONFIGURATIONS (remove if necessary)
	INTE = 1; 	INTF = 0;		//TMR0 Interrupt
	T0IE = 1;	T0IF = 0;	//TMR0 overflow interrupt
	
	TMR1IE = 1;	TMR1IF = 0;		//TMR1 Interrupt
	TMR2IE = 1;	TMR2IF = 0;		//TMR2 Interrupt
	CCP1IE = 1; CCP1IF = 0;		//CCP1 interrupt
	CCP2IE = 1; CCP2IF = 0;		//CCP1 interrupt
	ADIE = 1;	ADIF = 0;		//A/D Interrupt
	TXIE = 1;	TXIF = 0;		//USART Transmit Interrupt
	RCIE = 1; 	RCIF = 0;		//USART Receive Interrupt
	
	PEIE = 1; 	//PERIPHERAL INTERRUPT
	GIE = 1;	//GLOBAL INTERRUPT
	
	//USART CONFIGURATIONS

}

void interrupt ISR(){	// DESC: INTERRUPT FUNCTION
	GIE = 0;
	GIE = 1;
}

/*
void interrupt ISR(){	// DESC: interrupt module for I2C - edit/remove if needed 
	CKP = 0; // hold clock low (SSPCON reg)
	if(WCOL || SSPOV) // check if overflow or data collision (SSPCON reg)
	{
		temp = SSPBUF; // read SSPBUF to clear buffer
		WCOL = 0; // clear data collision flag
		SSPOV = 0; // clear overflow flag
		CKP = 1; // release clock (SSPCON reg)
	}
	
	if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) // write to slave
	{
		temp = SSPBUF; // read SSPBUF to clear buffer
		while(!BF); // wait until receive is complete (SSPSTAT reg)
		data = SSPBUF;	// read data from SSPBUF
		CKP = 1; // release clock (SSPCON reg)
	}
	else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) // read from slave
	{
		temp = SSPBUF; // read SSPBUF to clear buffer
		BF = 0; // clear buffer status bit (SSPSTAT reg)
		SSPBUF = data;	// send data by writing to SSPBUF
		CKP = 1; // release clock (SSPCON reg)
		while(BF); // wait until transmit is complete (SSPSTAT reg)
	}
	
	SSPIF = 0; // clear interrupt flag
	
}
*/

void main(){			// DESC: MAIN FUNCTION
	initPIC();
	initLCD();
	
	//foreground routine
	while(1){
		
	}
}


