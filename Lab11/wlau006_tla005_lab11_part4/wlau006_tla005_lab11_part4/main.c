/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 11  Exercise 4
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <io.c>
#include <avr/io.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>


unsigned char writeFlag;

volatile unsigned char x;

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

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

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
unsigned char GetKeypadKey() {

	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('D'); }
	if (GetBit(PINC,1)==0) { return('#'); }
	if (GetBit(PINC,2)==0) { return('0'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('C'); }
	if (GetBit(PINC,1)==0) { return('9'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('7'); }

	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('B'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('5'); }
	if (GetBit(PINC,3)==0) { return('4'); }

	// Check keys in col 4
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('3'); }
	if (GetBit(PINC,2)==0) { return('2'); }
	if (GetBit(PINC,3)==0) { return('1'); }

	return('\0'); // default value

}
/*enum Output_States {Init, Assign};
	
int Output_tick(int state){
	switch(state){ //Transitions
		case Init:
			state = Assign;
			break;
		case Assign:
			state = Assign;
			break;
		default: state = Init;
	}
	switch(state){ //Actions
		case Init: break;
		case Assign:
		if (writeFlag)
		{
			LCD_WriteData(x); writeFlag = 0;
		}
 // All 5 LEDs on
		break;
	}
	return state;
}*/

enum Keypad_States{Keypad_init, Keypad_read, Keypad_wait};

int Keypad_tick(int state){
	switch(state){ //Transitions
		case Keypad_init:
		state = Keypad_read;
		break;
		case Keypad_read:
		if(x != '\0'){
			state = Keypad_wait;
		}else{
			state = Keypad_read;
		}
			//state = (x != '\0') ? Keypad_wait: Keypad_read;
		break;
		case Keypad_wait:
		if(GetKeypadKey() != '\0'){
			state = Keypad_wait;
			}else{
			state = Keypad_read;
		}
		break;
	}
	switch(state){
		case Keypad_init: break;
		case Keypad_read: x = GetKeypadKey(); if(x != '\0'){ LCD_WriteData(x);} break;
		case Keypad_wait: break;
		default: break;
	}
	return state;
}
	
int main(void)
{
	unsigned long int scroll_calc = 50;
	//unsigned long int output_calc = 200;
	unsigned long int tmpGCD = 0;
	tmpGCD = findGCD(scroll_calc, scroll_calc);//, output_calc);
	unsigned long int GCD = tmpGCD;
	unsigned long int scroll_period = scroll_calc/GCD;
	//unsigned long int output_period = output_calc/GCD;
	
	static task task1;//, task2;
	task *tasks[] = { &task1};//, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	unsigned char i;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00; // PORTD set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1
	task1.state = Keypad_init;
	task1.period = scroll_period;
	task1.elapsedTime = scroll_period;
	task1.TickFct = &Keypad_tick;
	/*task2.state = Init;
	task2.period = output_period;
	task2.elapsedTime = output_period;
	task2.TickFct = &Output_tick;*/
	LCD_init();
	LCD_DisplayString(1,"Congratulations!");
	LCD_Cursor(1);
	TimerSet(GCD);
	TimerOn();
//			LCD_DisplayString(1, "Hello World");
	while(1) {
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}

		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}


