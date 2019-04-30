/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 8  Exercise 3
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

// from part 2
const unsigned char MAX;

int main(void)
{
    ADC_init();
	unsigned short x = ADC;  // Value of ADC register now stored in variable x.
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpB = 0; 
    while (1) 
    {
		if(x >= MAX/2){
			tmpB = 1;
		}else{
		tmpB = 1;
		}
		PORTB = tmpB;
		x = ADC
    }
}

