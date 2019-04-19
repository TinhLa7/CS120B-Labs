/*
 * lab_chip.c
 *
 * Created: 4/17/2019 8:40:51 AM
 * Author : tinhl
 */ 

#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	while(1)
	{
		PORTB = 0x0F; // Writes port B's 8 pins with 00001111
	}
}


