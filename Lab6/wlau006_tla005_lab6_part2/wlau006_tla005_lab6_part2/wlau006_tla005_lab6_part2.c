/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 6  Exercise 2 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define A0 (PINA & 0x01)

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	
	SREG != 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M; 
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States{/*Start,*/Init, LED1, LED1Wait, LED2, LED2Wait, LED3, LED3Wait} state;

void synchSM(){
	switch(state){ // State transitions
		case Init: state = LED1; break;
		case LED1: 
			if(A0) { // if button is pressed go to next LED 1 state
				state = LED1Wait;
			}else{
			state = LED2;
			}
			break;
		case LED1Wait:
			while(A0); // if button is continued to be pressed enter this while loop (because there is a timer, we can't simply set state = LED1Wait
			while(!A0); // if button has been released the program will cycle in this loop until A0 is pressed again to restart the game
			if(A0) state = Init;
			break;
		case LED2: 
			if(A0) { // if button is pressed go to next LED 1 state
				state = LED2Wait;
			}else{
			state = LED3; 
			}
			break;
		case LED2Wait:
			while(A0); // if button is continued to be pressed enter this while loop (because there is a timer, we can't simply set state = LED1Wait
			while(!A0); // if button has been released the program will cycle in this loop until A0 is pressed again to restart the game
			if(A0) state = Init;
			break;
		case LED3: 
			if(A0) { // if button is pressed go to next LED 1 state
				state = LED1Wait;
			}else{
			state = LED1; 
			}
			break;
		case LED3Wait:
			while(A0); // if button is continued to be pressed enter this while loop (because there is a timer, we can't simply set state = LED1Wait
			while(!A0); // if button has been released the program will cycle in this loop until A0 is pressed again to restart the game
			if(A0) state = Init;
		break;
		default: state = Init;
	}
	switch(state){ // State actions
		case Init: 
			tmpB = 0;
			break;
		case LED1:
			tmpB = 0;
			tmpB = 1;
			break;
		case LED1Wait: break;
		case LED2Wait: break;
		case LED3Wait: break;
		case LED2:
			tmpB = 0;
			tmpB = 2;
			break;
		case LED3:
			tmpB = 0;
			tmpB = 4;
			break;
		default: break;
	}
	PORTC = state;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300); // Set timer here
	TimerOn();
	state = Init;
    while (1) 
    {
		// User code (i.e. synchSM calls)
		synchSM();
		PORTB = tmpB;
		while(!TimerFlag); // Wait 1 sec - Note: for when actually programming Micro-controller
		//TimerISR(); // For Simulator Only
		TimerFlag = 0;
    }
}

