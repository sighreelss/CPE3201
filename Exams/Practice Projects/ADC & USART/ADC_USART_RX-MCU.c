#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


void main() {
   /* SET TO RECEIVER */
   SPBRG = 0x19;
   SYNC = 0;
   SPEN = 1;
   BRGH = 1;
   RX9 = 0;
   CREN = 1;
   
   
   TRISB = 0x00;
   TRISD = 0x00;
   PORTD = 0x00;
   PORTB = 0x00;
   
   for (;;) {
      if (RCIF) {
		while (!RCIF);
		PORTB = RCREG;
      }
   }
}