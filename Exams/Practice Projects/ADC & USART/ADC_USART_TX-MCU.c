#include <xc.h>
#include<stdio.h> 
#include <math.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF 	
#pragma config CP = OFF

int RB0_flag = 0;
int ADC_flag = 0;

void delay(int cnt){
	while(cnt--);
	return;
}

void init_PIC(){
	TRISB = 0x0F;
	TRISD = 0x00;
	
	//ADC
	ADCON1 = 0x80; 	// result: right Justified, clock: FOSC/2 
					// all ports in PORTA are analog 
					// VREF+: VDD, VREF-: VSS 
	ADCON0 = 0x41; 	// clock: FOSC/8, analog channel: AN0, 
					// A/D conversion: STOP, A/D module: ON 
	
	//INTERRUPTS
	ADIF = 0;
	ADIE = 1;
	PEIE = 1;
	INTEDG = 1; // interrupt at rising edge
	INTE = 1; // enable RB0/INT external interrupt
	INTF = 0; // clears the interrupt flag
	GIE = 1;
	
	/* SET TO SENDER */
	SPBRG = 0x19;
	SYNC = 0;
	SPEN = 1;
	BRGH = 1;
	RX9 = 0;
	TXEN = 1;
	
	return;
}

void instCtrl(unsigned char INST) {
	PORTD = INST;
	RB5 = 0;
	RB6 = 0;
	RB7 = 1;
	delay(1000);
	RB7 = 0;
	
	return;
}

void dataCtrl(unsigned char DATA) {
	PORTD = DATA;
	RB5 = 1;
	RB6 = 0;
	RB7 = 1;
	delay(1000);
	RB7 = 0;
	
	return;
}

void init_LCD(){
	instCtrl(0x38);
	instCtrl(0x0C);
	instCtrl(0x06);
	instCtrl(0x01);
	return;
}

void interrupt ISR(void)
{
	GIE = 0; 		// disable all unmasked interrupts (INTCON reg) 

	if(ADIF) 	// checks CCP1 interrupt flag 
	{
		ADIF=0;
		ADC_flag = 1;
	}
	
	if(INTF){
		INTF = 0;
		RB0_flag = 1;
	}
	
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
}


void main(){
	init_PIC();
	init_LCD();
	
	delay(1000);
	GO = 1;
	
	int DVAL = 0;
	int dec = 0;
	int units = 0;
	
	while(1){
		if (ADC_flag){
			ADC_flag = 0;
			DVAL = ADRESH;
			DVAL = DVAL << 8;
			DVAL = DVAL | ADRESL;

			units = (int)round(DVAL/205);
			dec = (int)round((DVAL % 205)/20.48);
			if (dec == 0x0A){
				dec = 0;
				units++;
			}
	
		}
		
		if(RB0_flag){
			RB0_flag = 0;
			int keyVal = units << 4 | dec;
			while (!TRMT);
			TXREG = keyVal;
			delay(5000);
			RB0_flag = 0;
		}
		
		instCtrl(0x80);
		dataCtrl(units + '0');
		dataCtrl('.');
		dataCtrl(dec + '0');
		
		delay(1000);
		GO = 1;
	}
	return;
}