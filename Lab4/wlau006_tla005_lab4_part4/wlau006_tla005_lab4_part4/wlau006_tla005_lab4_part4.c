/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 4  Exercise 4
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
// Bit-access function
#include <avr/io.h>
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
enum Lock_States {Lock_wait, Lock_Unlock, Lock_Lock, Lock_s0, Lock_s1} Lock_State;
void TickFct_Lock()
{
	unsigned char tmpA = PINA & 0x87;
	unsigned char tmpB = PORTB & 0x07;
	switch(Lock_State) {   // Transitions
		case Lock_wait:  // Initial transition
		if(GetBit(tmpA,7) == 1 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_Lock;
		}else if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 1 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_s0;
		}else{
			Lock_State = Lock_wait;
		}
		break;
		
		case Lock_s0:
		if(GetBit(tmpA,7) == 1 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_Lock;	
		}else if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_s1;
		}
		break;
		
		case Lock_s1:
		if(GetBit(tmpA,7) == 1 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_Lock;
		}else if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 1 && GetBit(tmpA,0) == 0 && GetBit(tmpB,0) == 1){ // Lock Door if code is entered and door is unlocked
			Lock_State = Lock_Lock;
		}else if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 1 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_Unlock;
		}else if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 1){
			Lock_State = Lock_wait;
		}else if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 1 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_wait;
		}else{
			Lock_State = Lock_s1;
		}
		break;

		case Lock_Lock:
		if(GetBit(tmpA,7) == 0 && GetBit(tmpA,2) == 0 && GetBit(tmpA,1) == 0 && GetBit(tmpA,0) == 0){
			Lock_State = Lock_wait;
		}else{
		Lock_State = Lock_Lock;
		}
		break;
		
		case Lock_Unlock:
		Lock_State = Lock_wait;
		break;
		
		default:
		Lock_State = Lock_wait;
		break;
	} // Transitions

	switch(Lock_State) {   // State actions
		case Lock_wait:
		break;
		
		case Lock_s0:
		break;
		
		case Lock_s1:
		break;
		
		case Lock_Unlock:
		PORTB = (PORTB & 0x00) | 0x01;
		break;
		
		case Lock_Lock:
		PORTB = PORTB & 0x00;
		break;
		
		default:
		break;
	} // State actions
	PORTC = Lock_State;
}

int main() {
	
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00;// Configure port B's 8 pins as outputs, initialize to 0s
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	Lock_State = Lock_wait; // Indicates initial call
	while(1) {
		TickFct_Lock();
	}
}