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
		
		break;
		case First:
		
		break;
		case FirstPress:
		
		break;
		case Second:
		
		break;
		case SecondPress:
		
		break;
		default:
		break;
	}
}
int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

