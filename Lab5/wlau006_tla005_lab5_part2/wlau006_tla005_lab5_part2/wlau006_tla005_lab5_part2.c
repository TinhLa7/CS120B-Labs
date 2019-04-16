/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 5  Exercise 2
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum INCDEC_States {INCDEC_wait, INCDEC_reset, INC_s0, INC_s1, DEC_s0, DEC_s1} INCDEC_State;
void TickFct_INCDEC()
{	
	unsigned char tmpB = PORTB & 0xFF;
	unsigned char tmpA = PINA & 0x03;
	switch(INCDEC_State) {   // Transitions
		case INCDEC_wait:  // Initial transition
			if(tmpA == 0){
				INCDEC_State = INCDEC_wait;
			}else if(tmpA == 1){
				INCDEC_State = INC_s0;
			}else if(tmpA == 2){
				INCDEC_State = DEC_s0;
			}else if(tmpA == 3){
				INCDEC_State = INCDEC_reset;
			}
		break;

		case INC_s0:
		if(tmpA == 3){
			INCDEC_State = INCDEC_reset;
		}else if(tmpA == 2){
			INCDEC_State = DEC_s0;
		}else if(tmpA == 0){
			INCDEC_State = INCDEC_wait;
		}else{
			INCDEC_State = INC_s1;
		}
		break;

		case INC_s1:
		if ((tmpA & 0x01) == 0) {
			if(tmpA == 2){
				INCDEC_State = DEC_s0;
			}else{
				INCDEC_State = INCDEC_wait;
			}
		}else if(tmpA == 3){
			INCDEC_State = INCDEC_reset;
		}else{
			INCDEC_State = INC_s1;
		}
		break;
		
		case DEC_s0:
		if(tmpA == 3){
			INCDEC_State = INCDEC_reset;
		}else if (tmpA == 1){
			INCDEC_State = INC_s0;
		}else if (tmpA == 0){
			INCDEC_State = INCDEC_wait;
		}else{
			INCDEC_State = DEC_s1;
		}
		break;

		case DEC_s1:
		if ((tmpA & 0x02) == 0) {
			if(tmpA == 1){
				INCDEC_State = INC_s0;
			}else{
				INCDEC_State = INCDEC_wait;
			}
		}else if(tmpA == 3){
			INCDEC_State = INCDEC_reset;
		}else{
			INCDEC_State = DEC_s1;
		}
		
		break;
		
		case INCDEC_reset:
		if(tmpA == 0){
			INCDEC_State = INCDEC_wait;
		}else{
			INCDEC_State = INCDEC_reset;
		}
		break;
		
		default:
		INCDEC_State = INCDEC_wait;
		break;
	} // Transitions

	switch(INCDEC_State) {   // State actions
		case INC_s0:
		if(tmpB < 9){
			tmpB +=1;
		}
		break;

		case INC_s1:
		break;
		
		case DEC_s0:
		if(tmpB > 0){
			tmpB -= 1;
		}
		break;
		
		case DEC_s1:
		break;
		
		case INCDEC_reset:
			tmpB = 0;
		break;
		
		case INCDEC_wait:
		break;

		default:
		break;
	} // State actions
	PORTB = tmpB;
}

int main() {
	
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	INCDEC_State = INCDEC_wait; // Indicates initial call
	PORTB = 0x00;
	while(1) {
		TickFct_INCDEC();
	}
}
