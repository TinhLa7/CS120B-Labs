/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 3  Exercise 2 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */



#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A	
	unsigned char tmpB = 0x00;
	while(1)
	{
		// 1) Read input
		tmpA = PINA & 0x0F;
		tmpB = PINA & 0x70;
		//2) Perform computation
		if(tmpA == 0){
			PORTC = (PORTC & 0x00) | 0x40;
		}else if(tmpA >=1 && tmpA <= 2){
			PORTC = (PORTC & 0x00) | 0x60;	
		}else if(tmpA >= 3 && tmpA <= 4){
			PORTC = (PORTC & 0x00) | 0x70;
		}else if(tmpA >= 5 && tmpA <= 6){
			PORTC = (PORTC & 0x00) | 0x38;
		}else if(tmpA >= 7 && tmpA <= 9){
			PORTC = (PORTC & 0x00) | 0x3C;
		}else if(tmpA >= 10 && tmpA <= 12){
			PORTC = (PORTC & 0x00) | 0x3E;
		}else if(tmpA >= 13 && tmpA <= 15){
			PORTC = (PORTC & 0x00) | 0x3F;
		}
		if(tmpB == 0x30){
			PORTC = PORTC | 0x80;
		}else{
			PORTC = PORTC & 0x7F;
		}
		
	}
	return 0;
}