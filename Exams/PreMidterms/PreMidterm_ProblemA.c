#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

int count_flag = 0;
int direction_flag = 0;
int setDuration = 122;
int keypad_flag = 0;


//delay_LCD - delay used to configure LCD
void delay_LCD(int max){
	int cnt;
	for(cnt=0;cnt<max;cnt++); // delay	

	return;
}

//delay_Duration - delay used to vary the duration of the display
void delay_Duration(int max){
	int cnt = 0;
	while (cnt < max){
		if(count_flag == 1){
			count_flag = 0;
			cnt++;
		}	
	}
	return;
}


void instCtrl(unsigned char data){
	PORTC = data;
	PORTB = 0x80;
	delay_LCD(100);
	PORTB = 0x00;

	return;
}

void dataCtrl(unsigned char data){
	PORTC = data;
	PORTB = 0xA0;
	delay_LCD(100);
	PORTB = 0x20;

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

void interrupt ISR()
{
	GIE = 0; // disables all unmasked interrupts to prevent interrupt overlap
	if (INTF) // check the interrupt flag
	{
		INTF = 0; // clears the interrupt flag
		/* write the interrupt service routine here
		for RB0/INT external interrupt */
		
		unsigned char input = PORTD & 0x0F;
		switch(input){	
			case 0x00: setDuration = 12; break;
			case 0x01: setDuration = 24; break;
			case 0x02: setDuration = 37; break;
			case 0x04: setDuration = 49; break;
			case 0x05: setDuration = 61; break;
			case 0x06: setDuration = 73; break;
			case 0x08: setDuration = 85; break;
			case 0x09: setDuration = 98; break;
			case 0x0A: setDuration = 110; break;
			case 0x0C: direction_flag = 1; break;
			case 0x0D: setDuration = 122; break;
			case 0x0E: direction_flag = 0; break;
		}
		keypad_flag = 1;
	}
	else if(T0IF) // check the interrupt flag for Timer0
	{
		T0IF = 0; // clears the interrupt flag
		/* write the interrupt service routine here for Timer0 overflow interrupt */
		count_flag = 1; // this is a global variable which will be
		// in the main routine (toggle)
	}
	GIE = 1; // enable interrupts again
	GIE = 1; // enable interrupts again
}


void main(){
	int LSD = 0x30;
	int MSD = 0x30;
	//INTERRUPT VARIABLES
	OPTION_REG = 0x04; 	// PS2:PS0 - prescaler 1:32
						// PSA - prescaler assignment to TMR0
						// T0CS - internal instruction cycle clock
						// T0SE - not used since clock is internal
	T0IE = 1; 		// enable Timer0 overflow interrupt
	T0IF = 0; 		// clears the interrupt flag
	INTEDG = 1; 	// interrupt at rising edge
	INTE = 1; 		// enable RB0/INT external interrupt
	INTF = 0; 		// clears the interrupt flag
	GIE = 1; 		// enables all unmasked interrupt

	//PIC16F877A SETUP
	TRISB = 0x01; 	// set first bit of PORTB as interrupt input
	TRISC = 0x00; 	// set all bits (port) in PORTC as output
	TRISD = 0x0F;	// sets the lower nibble as inputs
	
	initLCD();
	int delay = setDuration;
	while (1){
		while (!direction_flag){
			if (keypad_flag){
				delay = setDuration;
				keypad_flag = 0;
			}
			instCtrl(0x81);
			dataCtrl(LSD);
			instCtrl(0x80);
			dataCtrl(MSD);
			delay_Duration(delay);
			if (LSD == '9'){
				LSD = '0';
				if (MSD == '9'){
					MSD = '0';
					break;
				}
				MSD++;
				break;
			}
			LSD++;
		}

		while (direction_flag){
			if (keypad_flag){
				delay = setDuration;
				keypad_flag = 0;
			}
			instCtrl(0x81);
			dataCtrl(LSD);
			instCtrl(0x80);
			dataCtrl(MSD);
			delay_Duration(delay);
			if (LSD == '0'){
				LSD = '9';
				if (MSD == '0'){
					MSD = '9';
					break;
				}
				MSD--;
				break;
			}
			LSD--;
		}
	}
	
}