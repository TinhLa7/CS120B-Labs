/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 11  Exercise 1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <ucr/bit.h>

unsigned char x = 0;
unsigned char tmpB = 0;
enum Keypad_SM_States {Init, Wait, Assign} state;
	
Keypad_SM_tick(){
	switch(state){ //Transitions
		case Init:
			state = Wait;
			break;
		case Wait:
			state = (x) ? Assign : Wait;
			break;
		case Assign:
			state = Wait;
			break;
		default: state = Init;
	}
	switch(state){ //Actions
		case Init: break;
		case Wait:
			x = GetKeypadKey();
			break;
		case Assign:
			tmpB = 0;
			switch (x) {
				case '\0': tmpB = 0x1F; break; // All 5 LEDs on
				case '1': tmpB = 0x01; break; // hex equivalent
				case '2': tmpB = 0x02; break;
				case '3': tmpB = 0x03; break;
				case '4': tmpB = 0x04; break;
				case '5': tmpB = 0x05; break;
				case '6': tmpB = 0x06; break;
				case '7': tmpB = 0x07; break;
				case '8': tmpB = 0x08; break;
				case '9': tmpB = 0x09; break;
				case 'A': tmpB = 0x0A; break;
				case 'B': tmpB = 0x0B; break;
				case 'C': tmpB = 0x0C; break;
				case 'D': tmpB = 0x0D; break;
				case '*': tmpB = 0x0E; break;
				case '0': tmpB = 0x00; break;
				case '#': tmpB = 0x0F; break;
				default: tmpB = 0x1B; break; // Should never occur. Middle LED off.
			break;
	}
	PORTB = tmpB;
}

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/
unsigned char GetKeypadKey() {

	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }

	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	// Check keys in col 4	
	// ...
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }

	return('\0'); // default value

}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	while(1) {

		}
	}
}


