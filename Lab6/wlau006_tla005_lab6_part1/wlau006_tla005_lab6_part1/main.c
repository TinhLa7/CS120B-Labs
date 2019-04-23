/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 6  Exercise 1 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

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

enum States{/*Start,*/Init, LED1, LED2, LED3} state;

void synchSM(){
	switch(state){ // State transitions
		case Init: state = LED1; break;
		case LED1: state = LED2; break;
		case LED2: state = LED3; break;
		case LED3: state = LED1; break;
		default: state = Init;
	}
	switch(state){ // State actions
		case Init: break;
		case LED1:
			tmpB = 0;
			tmpB = 1;
			break;
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
	//DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	state = Init;
    while (1) 
    {
		// User code (i.e. synchSM calls)
		synchSM();
		PORTB = tmpB;
		while(!TimerFlag); // Wait 1 sec - Note: for when actually programming Microcontroller
		//TimerISR(); // For Simulator Only
		TimerFlag = 0;
    }
}

