/*
 * tla005_lab2_part1.c
 *
 * Created: 4/3/2019 9:06:29 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
    while(1)
    {
		tmpA = PINA & 0x15;
	    if((tmpA & 0x01) == 0x01){ 
			tmpB = tmpA;	
		}
		else{
		tmpB = 0x00;	
		}
		PORTB = tmpB;
    }
    return 0;
}

