/*
 * tla005_lab2_part2.c
 *
 * Created: 4/3/2019 2:31:42 PM
 * Author : tinhl
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
    unsigned char cntavail = 0x00;
	while(1)
    {
		cntavail = 0x00;
	    // 1) Read input
	    tmpA = PINA & 0x0F;
	    // 2) Perform computation
	    tmpB = tmpA & 0x01;
		if(tmpB == 0)
		{
			cntavail = cntavail + 1;	
		}
		tmpB = tmpA & 0x02;
		if(tmpB == 0)
		{
			cntavail = cntavail + 1;
		}
		tmpB = tmpA & 0x04;
		if(tmpB == 0)
		{
			cntavail = cntavail + 1;
		}
		tmpB = tmpA & 0x08;
		if(tmpB == 0)
		{
			cntavail = cntavail + 1;
		}
		PORTC = cntavail;
    }
    return 0;
}

