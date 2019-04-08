
/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 3  Exercise 1 
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
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;
	unsigned char cntavail = 0x00; //Temp variable to save number of open spaces
	
	while(1)
	{
		// 1) Read input
		cntavail = 0;
		tmpA = PINA & 0xFF;
		tmpB = PINB & 0xFF;
		cntavail += GetBit(tmpA, 0);
		cntavail += GetBit(tmpB, 0);
		cntavail += GetBit(tmpA, 1);
		cntavail += GetBit(tmpB, 1);
		cntavail += GetBit(tmpA, 2);
		cntavail += GetBit(tmpB, 2);
		cntavail += GetBit(tmpA, 3);
		cntavail += GetBit(tmpB, 3);
		cntavail += GetBit(tmpA, 4);
		cntavail += GetBit(tmpB, 4);
		cntavail += GetBit(tmpA, 5);
		cntavail += GetBit(tmpB, 5);
		cntavail += GetBit(tmpA, 6);
		cntavail += GetBit(tmpB, 6);
		cntavail += GetBit(tmpA, 7);
		cntavail += GetBit(tmpB, 7);
		// 2) Perform computation
		PORTC = cntavail;
	}
	return 0;
}