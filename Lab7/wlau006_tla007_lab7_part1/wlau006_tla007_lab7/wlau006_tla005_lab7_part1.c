/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 7  Exercise 1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned char tmpC = 0x00;
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

void syncSM()
{	
	unsigned char tmpA = PINA & 0x03;
	switch(INCDEC_State) {   // Transitions
		case INCDEC_init:  // Initial transition
			if(tmpA == 0){
				INCDEC_State = INCDEC_init;
			}else if(tmpA == 1){
				INCDEC_State = INC_s0;
			}else if(tmpA == 2){
				INCDEC_State = DEC_s0;
			}else if(tmpA == 3){
				INCDEC_State = INCDEC_reset0;
			}
		break;

		case INC_s0:
		if(tmpA == 3){
			INCDEC_State = INCDEC_reset0;
		}else if(tmpA == 2){
			INCDEC_State = DEC_s0;
		}else if(tmpA == 0){
			INCDEC_State = INCDEC_init;
		}else{
			INCDEC_State = INC_s0;
		}
		break;

		/*case INC_s1:
		if ((tmpA & 0x01) == 0) {
			if(tmpA == 2){
				INCDEC_State = DEC_s0;
			}else{
				INCDEC_State = INCDEC_wait;
			}
		}else if(tmpA == 3){
			INCDEC_State = INCDEC_reset0;
		}else{
			INCDEC_State = INC_s1;
		}
		break;*/
		
		case DEC_s0:
		if(tmpA == 3){
			INCDEC_State = INCDEC_reset0;
		}else if (tmpA == 1){
			INCDEC_State = INC_s0;
		}else if (tmpA == 0){
			INCDEC_State = INCDEC_init;
		}else{
			INCDEC_State = DEC_s0;
		}
		break;

		/*case DEC_s1:
		if ((tmpA & 0x02) == 0) {
			if(tmpA == 1){
				INCDEC_State = INC_s0;
			}else{
				INCDEC_State = INCDEC_wait;
			}
		}else if(tmpA == 3){
			INCDEC_State = INCDEC_reset0;
		}else{
			INCDEC_State = DEC_s1;
		}
		
		break;*/
		
		case INCDEC_reset0:
		if(tmpA == 0){
			INCDEC_State = INCDEC_init;
		}else{
			INCDEC_State = INCDEC_reset1;
		}
		break;
		
		case INCDEC_reset1:
		if(tmpA == 0){
			INCDEC_State = INCDEC_init;
		}else if (tmpA == 1){
			INCDEC_State = INC_s0;
		}else if (tmpA == 2){
			INCDEC_State = DEC_s0;
		}else{
			INCDEC_State = INCDEC_reset1;
		}
		break;		
		
		default:
		INCDEC_State = INCDEC_init
		break;
	} // Transitions

	switch(INCDEC_State) {   // State actions
		case INC_s0:
		if(tmpC < 9){
			tmpC +=1;
		}
		LCD_WriteData(tmpC + '0');
		while(!TimerFlag){
			tmpA = PINA & 0x03;
			if(tmpA != 1){
				TimerFlag = 0;
				break;
			}
		} // Wait 1 sec - Note: for when actually programming Microcontroller
		//TimerISR(); // For Simulator Only
		TimerFlag = 0;
		break;
		
		case DEC_s0:
		if(tmpC > 0){
			tmpC -= 1;
		}
		LCD_WriteData(tmpC + '0');
		while(!TimerFlag){
			tmpA = PINA & 0x03;
			if(tmpA != 2){
				TimerFlag = 0;
				break;
			}
		} // Wait 1 sec - Note: for when actually programming Microcontroller
		//TimerISR(); // For Simulator Only
		TimerFlag = 0;
		break;
		
		case INCDEC_reset0:
			LCD_WriteData(0 + '0');
		break;
		
		case INCDEC_init:
		break;

		default:
		break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 1s
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
		
		// Initializes the LCD display
		LCD_init();
		TimerSet(1000);
		TimerOn();
		state = INCDEC_init;
		LCD_WriteData(0 + '0')
		while (1)
		{
			// User code (i.e. synchSM calls)
			synchSM();
		}
	}
}
enum INCDEC_States {INCDEC_init, INCDEC_reset0, INCDEC_reset1, INC_s0, DEC_s0} INCDEC_State;
	

