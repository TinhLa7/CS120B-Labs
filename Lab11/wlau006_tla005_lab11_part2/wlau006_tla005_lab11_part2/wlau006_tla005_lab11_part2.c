/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 11  Exercise 2
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <io.c>
#include <avr/io.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>

#define LCD_SIZE 16
#define STRING_SIZE 38
#define SEPARATION 3
volatile unsigned char inputs[LCD_SIZE];
volatile unsigned char inputstring[STRING_SIZE] = "CS120B is Legend, wait for it... dary!";

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

enum Output_States {Init, Assign};
	
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
			for(unsigned char i = 1; i <= LCD_SIZE; i++){
				if(inputs[i-1] != '\0'){
					LCD_Cursor(i); LCD_WriteData(inputs[i - 1]);
				}else{
					LCD_Cursor(i); LCD_WriteData(' ');
				}
			}
		break;
	}
	return state;
}

enum TextScroll_States{TextScroll_init, TextScroll_start, TextScroll_scroll};
	
int TextScroll_tick(int state){
	static unsigned char index;
	switch(state){ //Transitions
		case TextScroll_init:
			state = TextScroll_start;
		break;
		case TextScroll_start:
			state = TextScroll_scroll;
		break;
		
		case TextScroll_scroll:
			if(index > STRING_SIZE + SEPARATION){
				index = 0;
				state = TextScroll_start;
			}else{
				state = TextScroll_scroll;
			}
		break;
	}
	switch(state){
		case TextScroll_init:
		break;
		
		case TextScroll_start:
		inputs[LCD_SIZE - 1] = inputstring[0];
		index++;
		break;
		
		case TextScroll_scroll:
		inputs[0] = inputs[1];
		inputs[1] = inputs[2];
		inputs[2] = inputs[3];
		inputs[3] = inputs[4];
		inputs[4] = inputs[5];
		inputs[5] = inputs[6];
		inputs[6] = inputs[7];
		inputs[7] = inputs[8];
		inputs[8] = inputs[9];
		inputs[9] = inputs[10];
		inputs[10] = inputs[11];
		inputs[11] = inputs[12];
		inputs[12] = inputs[13];
		inputs[13] = inputs[14];
		inputs[14] = inputs[15];
		if(index >= STRING_SIZE){
		inputs[15] = '\0';
		}else{
		inputs[15] = inputstring[index];
		}
		index++;
		break;
		
		default: break;
	}
	return state;
}
	
int main(void)
{
	unsigned long int scroll_calc = 200;
	unsigned long int output_calc = 200;
	unsigned long int tmpGCD = 0;
	tmpGCD = findGCD(scroll_calc, output_calc);
	unsigned long int GCD = tmpGCD;
	unsigned long int scroll_period = scroll_calc/GCD;
	unsigned long int output_period = output_calc/GCD;
	
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	unsigned char i;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00; // PORTD set to output, outputs init 0s
//	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1
	task1.state = TextScroll_init;
	task1.period = scroll_period;
	task1.elapsedTime = scroll_period;
	task1.TickFct = &TextScroll_tick;
	task2.state = Init;
	task2.period = output_period;
	task2.elapsedTime = output_period;
	task2.TickFct = &Output_tick;
	LCD_init();
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


