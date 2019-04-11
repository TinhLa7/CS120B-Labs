/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 4  Exercise 1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum LED_States { LED_start, LED_s0, LED_s1, LED_s2, LED_s3 } LED_State;

void TickFct_LED()
{
	unsigned char tmpA = PINA & 0x01;
	switch(LED_State) {   // Transitions
		case LED_start:  // Initial transitiona
		LED_State = LED_s0;
		break;

		case LED_s0:
		if (tmpA == 1) {
			LED_State = LED_s1;
		}else{
			LED_State = LED_s0;
		}
		break;

		case LED_s1:
		if (tmpA == 0) {
			LED_State = LED_s2;
		}else{
			LED_State = LED_s1;
		}
		break;
		
		case LED_s2:
		if (tmpA == 1) {
			LED_State = LED_s3;
			}else{
			LED_State = LED_s2;
		}
		break;

		case LED_s3:
		if (tmpA == 0) {
			LED_State = LED_s0;
			}else{
			LED_State = LED_s3;
		}
		break;
		default:
		LED_State = LED_start;
		break;
	} // Transitions

	switch(LED_State) {   // State actions
		case LED_s0:
		PORTB = (PORTB & 0x00) | 0x01;
		break;

		case LED_s1:
		break;
		
		case LED_s2:
		PORTB = (PORTB & 0x00) | 0x02;
		break;
		
		case LED_s3:
		break;
		
		default:
		break;
	} // State actions
}

int main() {
	
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s

	LED_State = LED_start; // Indicates initial call

	while(1) {
		TickFct_LED();
	}
}
