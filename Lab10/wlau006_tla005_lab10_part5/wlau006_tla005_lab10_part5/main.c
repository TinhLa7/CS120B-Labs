/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 10  Exercise 5
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

// Global variables
#define TASKSSIZE 3
unsigned char tmpB;
unsigned char tmpA;

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
typedef struct task {
	int state;				// Task’s current state
	unsigned long period;			// Task period
	unsigned long elapsedTime;		// Time elapsed since last task tick
	int (*TickFct)(int);			// Task tick function
} task;

task tasks[TASKSSIZE];
enum Button_states{button_start, read};
enum INCDEC_States {incdec_start, stop, reset, INC, DEC};
int TickFct_button(int state){
	tmpA = PINA;
	switch(state){
		case button_start:
		state = read;
		break;
		case read:
			state = read;
		break;
	}
	switch(state){
		case button_start:
		break;
		case read:
		if(!GetBit(tmpA,0) && !GetBit(tmpA,1)){
			tasks[1].state = stop;
			tasks[1].elapsedTime = 1000;
		}else if(GetBit(tmpA,0) && !GetBit(tmpA,1)){
			tasks[1].state = INC;
		}else if(!GetBit(tmpA,0) && GetBit(tmpA,1)){
			tasks[1].state = DEC;
		}else if(GetBit(tmpA,0) && GetBit(tmpA,1)){
			tasks[1].state = reset;
			tasks[1].elapsedTime = 1000;
		}
		break;
	}
	return state;
};
unsigned char count;
int TickFct_INCDEC(int state)
{
	tmpB = PINB;
	switch(state) {   // Actions
		case incdec_start:
			state = stop;
		break;
		case stop:
			count = 0;
			tasks[1].period = 1;
		break;
		case reset:
			tmpB = 0;
		break;
		case INC:
			if(tasks[1].period == 1){
				tasks[1].period = 1000;
			}
			if(count == 3){
				tasks[1].period = 400;
			}
			count++;
			if(tmpB < 9){
				tmpB++;
			}
		break;
		case DEC:
			if(tasks[1].period == 1){
				tasks[1].period = 1000;
			}
			if(count == 3){
				tasks[1].period = 400;
			}
			count++;
			if(tmpB > 0){
				tmpB--;
			}
		break;
	}
	return state;
}

enum OUTPUT { SMStart, SM_OUTPUT };
int TickFct_OUTPUT(int state){
	switch(state){// Transitions
		case SMStart:
			state = SM_OUTPUT;
			break;
		case SM_OUTPUT:
			state = SM_OUTPUT;
			break;
	}
	switch(state){// Actions
		case SMStart: break;
		case SM_OUTPUT:
			PORTB = tmpB;
		break;
		default:break;
	}
	return state;
};

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

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerISR() {
	unsigned char i;
	for (i = 0;i < TASKSSIZE;++i) {
		if (tasks[i].elapsedTime >= tasks[i].period) {
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime++;
	}
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

int main() {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; 
	unsigned char i = 0;
	tasks[i].state = button_start;
	tasks[i].period = 200;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_button;
	i++;
	tasks[i].state = incdec_start;
	tasks[i].period = 1;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_INCDEC;
	i++;
	tasks[i].state = SMStart;
	tasks[i].period = 200;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_OUTPUT;
	TimerSet(1);
	TimerOn();
	while(1) {
		/*unsigned char i;
		for (i = 0;i < TASKSSIZE;++i) {
			if (tasks[i].elapsedTime >= tasks[i].period) {
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += tasks[i].period;
		}*/
	}
	return 0;
}