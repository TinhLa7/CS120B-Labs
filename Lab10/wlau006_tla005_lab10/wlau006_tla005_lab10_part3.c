/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 10  Exercise 3
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

enum BL_States { BL_SMStart, BL_LEDOff, BL_LEDON};
int TickFct_BlinkLED(int state) { 
	switch(state){ // Transitions
		case BL_SMStart:
			state = BL_LEDOff;
			break;
		case BL_LEDOff:
			state = BL_LEDON;
			break;
		case BL_LEDON:
			state = BL_LEDOff;
			break;
		default: 
			state = BL_SMStart;
			break;
	}
	switch(state){ // Actions
		case BL_SMStart:
			break;
		case BL_LEDOff:
			tmpB = SetBit(tmpB, 3, 0);
			break;
		case BL_LEDON:
			tmpB = SetBit(tmpB, 3, 1);
			break;
		default: break;
	}
	return state; }

enum TL_States { TL_SMStart, TL_Seq0, TL_Seq1, TL_Seq2 };
int TickFct_ThreeLED(int state) { 
	switch(state){// Transitions
		case TL_SMStart:
			state = TL_Seq0;
			break;
		case TL_Seq0:
			state = TL_Seq1;
			break;
		case TL_Seq1:
			state = TL_Seq2;
			break;
		case TL_Seq2:
			state = TL_Seq0;
			break;
		default: 
			state = TL_SMStart;
			break;
	}
	switch(state){// Actions
		case TL_SMStart: break;
		case TL_Seq0:
			tmpB = SetBit(tmpB, 0, 1);
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 0);
		break;
		case TL_Seq1:
			tmpB = SetBit(tmpB, 0, 0);
			tmpB = SetBit(tmpB, 1, 1);
			tmpB = SetBit(tmpB, 2, 0);
		break;
		case TL_Seq2:
			tmpB = SetBit(tmpB, 0, 0);
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 1);
		break;
		default: break;
	}
	return state; }
	
enum OUTPUT { SMStart, SM_OUTPUTON, SM_OUTPUTOff };
int TickFct_OUTPUT(int state){
	tmpA = PINA;
	switch(state){// Transitions
		case SMStart:
			state = SM_OUTPUTON;
			break;
		case SM_OUTPUTON:
			state = SM_OUTPUTOff;
			break;
		case SM_OUTPUTOff:
			if(GetBit(tmpA, 2)){
				state = SM_OUTPUTON;
			}
	}
	switch(state){// Actions
		case SMStart: break;
		case SM_OUTPUTON:
			tmpB = SetBit(tmpB, 4, 1);
			PORTB = tmpB;
			break;
		case SM_OUTPUTOff:
			tmpB = SetBit(tmpB, 4, 0);
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

#define SYNCH_SM_TIMER 1
#define BL_SM_PERIOD 1000
#define TL_SM_PERIOD 300
#define OUTPUTSM_PERIOD 2

int main() {
	//long count = 0;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; 
	unsigned char i = 0;
	tasks[i].state = BL_SMStart;
	tasks[i].period = BL_SM_PERIOD;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_BlinkLED;
	i++;
	tasks[i].state = TL_SMStart;
	tasks[i].period = TL_SM_PERIOD;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ThreeLED;
	i++;
	tasks[i].state = SMStart;
	tasks[i].period = OUTPUTSM_PERIOD;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_OUTPUT;
	TimerSet(SYNCH_SM_TIMER);
	TimerOn();
	while(1) {
		// *NOTE*: Uncomment for Simulator only!
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