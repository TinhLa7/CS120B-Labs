/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 8  Exercise 4
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
	unsigned short threshold = MAX / 8;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpB = 0;
	while (1)
	{
		if( x > 0 && x <= threshold){
			tmpB = 0x01;
		}
		else if(x > threshold && x <= (2*threshold)){
			tmpB = 0x03;
		}
		else if(x > (2*threshold) && x <= (3*threshold)){
			tmpB = 0x07;
		}
		else if(x > (3*threshold) && x <= (4*threshold)){
			tmpB = 0x0F;
		}
		else if(x > (4*threshold) && x <= (5*threshold)){
			tmpB = 0x1F;
		}
		else if(x > (5*threshold) && x <= (6*threshold)){
			tmpB = 0x3F;
		}
		else if(x > (6*threshold) && x <= (7*threshold)){
			tmpB = 0x7F;
		}
		else if(x > (7*threshold) && x <= (8*threshold)){
			tmpB = 0xFF;
		}
		PORTB = tmpB;
		x = ADC
	}
}


