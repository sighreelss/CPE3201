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
int RB0_flag = 0;	//TMR0 flag bit


void initPIC(){			// DESC: INITIALIZES PIC
	//PORT CONFIGURATIONS (edit and/or remove if necessary)
	TRISB = 0x01;	PORTB = 0x00;	//PORTB config
	TRISD = 0x0F;	PORTD = 0x00;	//PORTD config
	
	//PERIPHERAL CONFIGURATIONS (edit and/or remove if necessary)
	INTEDG = 1; 		
	SPBRG = 0x19;	SYNC = 0;	BRGH = 1;	SPEN = 1; //USART CONFIGURATIONS
	TX9 = 0;	TXEN = 1;	//USART ASYNCH TRANSMIT
	
	//INTERRUPT CONFIGURATIONS (remove if necessary)
	INTE = 1; 	INTF = 0;		//TMR0 Interrupt
	GIE = 1;	//GLOBAL INTERRUPT
	
	//USART CONFIGURATIONS

}

void interrupt ISR(){	// DESC: INTERRUPT FUNCTION
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

	while(1){
		if (RB0_flag){
			while(!TRMT);
			TXREG = key_pressed();
			RB0_flag = 0;
		}
	}
}


