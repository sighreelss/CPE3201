// TITLE: 	MPLAB IDE PIC16F977A CODE TEMPLATE
// CREATOR:	CYRIL ANDRE DURANGO

// INCLUDE LINES UNDER HERE
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
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
int TMR0_flag = 0;	//TMR0 flag bit

//NECESSARY FUNCTIONS
void delay(int cnt){	// DESC: SETS PIC DELAY
	while(cnt--);
	return;
}

void initPIC(){			// DESC: INITIALIZES PIC
	//PORT CONFIGURATIONS (edit and/or remove if necessary)
	TRISB = 0x01;	PORTB = 0x00;	//PORTB config
	TRISD = 0x0F;	PORTD = 0x00;	//PORTD config
	
	//PERIPHERAL CONFIGURATIONS (edit and/or remove if necessary)
	INTEDG = 1; 	//TIMER0 CONFIG

	SPBRG = 0x19;	SYNC = 0;	BRGH = 1;	SPEN = 1; //USART CONFIGURATIONS
	TX9 = 0;	TXEN = 1;	//USART ASYNCH TRANSMIT
	
	//INTERRUPT CONFIGURATIONS (remove if necessary)
	INTE = 1; 	INTF = 0;		//TMR0 Interrupt
	
	PEIE = 1; 	//PERIPHERAL INTERRUPT
	GIE = 1;	//GLOBAL INTERRUPT
	
}

void interrupt ISR(){	// DESC: INTERRUPT FUNCTION
	GIE = 0;
	if(INTF){
		INTF = 0;
		TMR0_flag = 1;
	}
	GIE = 1;
}

void main(){			// DESC: MAIN FUNCTION
	initPIC();
	//foreground routine
	int keypad_input;
	while(1){
		if(TMR0_flag){
			TMR0_flag = 0;
			int input = (PORTD) & 0x0F;
			switch(input){	
				case 0x00: keypad_input = 0X01; break;
				case 0x01: keypad_input = 0X02; break;
				case 0x02: keypad_input = 0X03; break;
				case 0x04: keypad_input = 0X04; break;
				case 0x05: keypad_input = 0X05; break;
				case 0x06: keypad_input = 0X06; break;
				case 0x08: keypad_input = 0X07; break;
				case 0x09: keypad_input = 0X08; break;
				case 0x0A: keypad_input = 0X09; break;
				case 0x0C: keypad_input = 0x2A; break;
				case 0x0D: keypad_input = 0X00; break;
				case 0x0E: keypad_input = 0x2B; break;
				default: keypad_input = 0X0F; break;
			}

			TXREG = keypad_input;
		}

	}
}


