/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 6  Exercise 2 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 6  Exercise 1 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#define A0 (PINA & 0x01)

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned char tmpB = 0x00;
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
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

