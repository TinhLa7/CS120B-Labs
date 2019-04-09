/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 3  Exercise 4 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	DDRC = 0xFF; PORTC = 0x00;// Configure port B's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
    //unsigned char tmp = 0x00; // temporary variable to hold bit
    
	while (1) 
    {
		tmpA = PINA & 0xFF; // Read input off of PINA
		// PA7 -> PB3
		tmpB = SetBit(tmpB, 3, GetBit(tmpA,7));
		// PA6 -> PB2
		tmpB = SetBit(tmpB, 2, GetBit(tmpA,6));
		// PA5 -> PB1
		tmpB = SetBit(tmpB, 1, GetBit(tmpA,5));
		// PA4 -> PB0
		tmpB = SetBit(tmpB, 0, GetBit(tmpA,4));
		// PA3 -> PC7
		tmpC = SetBit(tmpC, 7, GetBit(tmpA,3));
		// PA2 -> PC6
		tmpC = SetBit(tmpC, 6, GetBit(tmpA,2));
		// PA1 -> PC5
		tmpC = SetBit(tmpC, 5, GetBit(tmpA,1));
		// PA0 -> PC4
		tmpC = SetBit(tmpC, 4, GetBit(tmpA,0));
		PORTB = tmpB;
		PORTC = tmpC;
    }
}

