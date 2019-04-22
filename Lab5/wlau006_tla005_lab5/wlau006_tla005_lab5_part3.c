/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 5  Exercise 3 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#define button GetBit(tmpA, 0)
//global variables
unsigned char tmpA;
unsigned char tmpB;

enum States {Start, Init, L1, L1Wait, L2, L2Wait, L3, L3Wait, L4, L4Wait, L5, L5Wait, L6, L6Wait, L7, L7Wait} state;

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

void state_func() {
	tmpA = PINA & 0xFF;
	switch(state){// transitions
		case Start:
			if(!button){
				state = Init;
			}else{
				state = Start;
			}
		break;
		
		case Init:
			if(!button){
				state = Init;
			}else{
				state = L1;
			}
		break;
		
		case L1:
			if(button){
				state = L1;
			}else{
				state = L1Wait;
			}
		break;
		
		case L1Wait:
			if(!button){
				state = L1Wait;
			}else{
				state = L2;
			}
		break;
		
		case L2:
			if(button){
				state = L2;
			}else{
				state = L2Wait;
		}
		break;
		
		case L2Wait:
			if(!button){
				state = L2Wait;
			}else{
				state = L3;
			}
		break;
		
		case L3:
		if(button){
			state = L3;
		}else{
			state = L3Wait;
		}
		break;
		
		case L3Wait:
		if(!button){
			state = L4Wait;
		}else{
			state = L4;
		}
		break;
		
		case L4:
		if(button){
			state = L4;
		}else{
			state = L4Wait;
		}
		break;
		
		case L4Wait:
		if(!button){
			state = L4Wait;
		}else{
			state = L5;
		}
		break;
		
		case L5:
		if(button){
			state = L5;
		}else{
			state = L5Wait;
		}
		break;
		
		case L5Wait:
		if(!button){
			state = L5Wait;
		}else{
			state = L6;
		}
		break;
		
		case L6:
		if(button){
			state = L6;
		}else{
			state = L6Wait;
		}
		break;
		
		case L6Wait:
		if(!button){
			state = L6Wait;
		}else{
			state = L7;
		}
		break;
		
		case L7:
		if(button){
			state = L7;
		}else{
			state = L7Wait;
		}
		break;
		
		case L7Wait:
		if(!button){
			state = L7Wait;
		}else{
			state = Start;
		}
		break;
		
		default:
		state = Start;
		break;
	}
	switch(state){//Actions
		case Start:
		break;
		
		case Init:
			tmpB = 0;
		break;	
		
		case L1:
			tmpB = SetBit(tmpB, 0, 1);
		break;
		
		case L1Wait:
		break;
		
		case L2:
			tmpB = SetBit(tmpB, 2, 1);
		break;
		
		case L2Wait:
		break;
		
		case L3:
		tmpB = SetBit(tmpB, 4, 1);
		break;
		
		case L3Wait:
		break;
		
		case L4:
		tmpB = 0;
		tmpB = SetBit(tmpB, 1, 1);
		tmpB = SetBit(tmpB, 3, 1);
		tmpB = SetBit(tmpB, 5, 1);
		break;
		
		case L4Wait:
		break;
		
		case L5:
		tmpB = 0;
		tmpB = 7;
		break;
		
		case L5Wait:
		break;
		
		case L6:
		tmpB = 0;
		tmpB = 0x38;
		break;
		
		case L6Wait:
		break;
		
		case L7:
		tmpB = 0;
		tmpB = 0xFF;
		break;
		
		case L7Wait:
		break;
		
		default:
		break;
	}
	PORTB = tmpB;
	PORTC = state;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	tmpA = 0;
	tmpB = 0;
	state = Start;
    while (1) 
    {
		state_func();
    }
}

