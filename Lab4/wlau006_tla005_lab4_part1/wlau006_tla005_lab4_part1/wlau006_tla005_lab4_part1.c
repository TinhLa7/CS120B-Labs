/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 4  Exercise 1 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

//global variables
unsigned char tmpA;
unsigned char tmpB;

enum States {Start, Init, First, FirstPress, Second, SecondPress} state;

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

#define button GetBit(tmpA, 0);

void state_func() {
	switch (state){ // Transitions
		case Start:
			state = Init;	
			break;
		case Init:
			state = First;
			break;
		case First:
			if(button){
				state = FirstPress;
			}else{
				state = First;
			}
			break;
		case FirstPress:
			if(!button){
				state = Second;
			}else{
				state = FirstPress;
			}
			break;
		case Second:
			if(button){
				state = SecondPress;
				}else{
					state = Second;
			}
			break;
		case SecondPress:
			if(!button){
				state = First;
				}else{
					state = SecondPress;
				}
				break;
		default:
			state = Init;
			break;
	} 
	
	switch (state) { // Actions
		case Start:
		break;
		case Init:
			tmpB = SetBit(tmpB, 0, 1);
			PORTB = tmpB;
		break;
		case First:
		break;
		case FirstPress:
			tmpB = SetBit(tmpB, 0, 0);
			tmpB = SetBit(tmpB, 1, 1);
			PORTB = tmpB;
		break;
		case Second:
		break;
		case SecondPress:
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 0, 1);
			PORTB = tmpB;
		break;
		default:
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    tmpA = 0x00;
	tmpB = 0x00;
    while (1) 
    {
		tmpA = PINA & 0x03;
		state = Start;
		state_func();
    }
}

