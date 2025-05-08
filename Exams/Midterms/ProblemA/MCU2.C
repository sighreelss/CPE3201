#include <xc.h>

//PRAGMA LINES UNDER HERE
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay(int max){
	int cnt;
	for(cnt=0;cnt<max;cnt++); // delay	

	return;
}

void instCtrl(unsigned char data){
	PORTB = data;
	PORTC = 0x02;
	delay(1000);
	PORTC = 0x00;

	return;
}

void dataCtrl(unsigned char data){
	PORTB = data;
	PORTC = 0x03;
	delay(1000);
	PORTC = 0x01;

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


void main(void) 
{ 
	SPBRG = 0x19;	// 9.6K baud rate @ FOSC=4MHz, asynchronous high speed 
					// (see formula in Table 10-1) 
	SYNC = 0; 		
	SPEN = 1; 		
	RX9 = 0; 		
	BRGH = 1; 		
	CREN = 1; 
		
	TRISB = 0x00; 
	TRISC = 0xF0;
	TRISD = 0x00;	
	PORTB = 0x00; 

	unsigned int data, output;
	long VAL = 0; 
	int i;
	int DATA[6] = {0};
		
	int temp = 0;

	int firstnum_flag = 0;
	int secondnum_flag = 0;
	int add_flag = 0;
	int multiply_flag = 0;
	
	long firstnum = 0;
	long secondnum = 0;
	int temp_1st_arr[3] = {0};
	int temp_2nd_arr[3] = {0};

	initLCD();
	instCtrl(0xA2);
	for(i = 5; i >= 0; i--){
		dataCtrl('-');
	}
	while(1) {
		instCtrl(0xA2);
		for(i = 5; i >= 0; i--){
			dataCtrl('-');
		}
		instCtrl(0xE2);
		for(i = 5; i >= 0; i--){
			dataCtrl(DATA[i] + '0');
		}
		
		while(!RCIF);
		temp = RCREG;
		if (firstnum_flag < 3 && temp < 10){
			instCtrl(0x91 + firstnum_flag);
			temp_1st_arr[firstnum_flag] = temp;
			dataCtrl(temp_1st_arr[firstnum_flag] + '0');
			firstnum = firstnum*10 + temp_1st_arr[firstnum_flag];
			firstnum_flag++;
		}
		
		else if (secondnum_flag < 3 && firstnum_flag == 3 && temp < 10){
			instCtrl(0xD1 + secondnum_flag);
			temp_2nd_arr[secondnum_flag] = temp;
			dataCtrl(temp_2nd_arr[secondnum_flag]+'0');
			secondnum = secondnum*10 + temp_2nd_arr[secondnum_flag];
			secondnum_flag++;
		}
	
		else if (firstnum_flag == 3 && secondnum_flag ==3){
			instCtrl(0xCE);
			if(temp == 0x2A){
				multiply_flag = 1;	
				dataCtrl('x');
				VAL = firstnum * secondnum;
			}
			else if(temp == 0x2B){
				add_flag = 1;
				dataCtrl('+');
				VAL = firstnum + secondnum;
			}
		} 
		
		if (add_flag || multiply_flag){
			for(i = 0; i < 6; i++){
				DATA[i] = VAL % 10;
				VAL = VAL/10;
			}
			firstnum = 0;
			secondnum = 0;	
			
			firstnum_flag = 0;
			secondnum_flag = 0;
			temp = 0;
			
			add_flag = 0;
			multiply_flag = 0;
		}
	}
} 
