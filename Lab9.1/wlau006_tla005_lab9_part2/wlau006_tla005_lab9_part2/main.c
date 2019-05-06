/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 9  Exercise 1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

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

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) { static double current_frequency;
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) {
			TCCR3B &= 0x08;
		} //stops timer/counter
		else {
			TCCR3B |= 0x03;
		} // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) {
			OCR3A = (int)0xFFFF;
		} // prevents OCR0A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) {
			OCR3A = 0x0000;
		} // set OCR3A based on desired frequency
		else {
			OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		}
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0); // COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
unsigned char tmpA;
enum tone_states{ tone_init, tone_1, tone_2, tone_3, tone_4, tone_5, tone_6, tone_7, tone_8} tone_state;
void syncSM1(){
	tmpA = PINA & 0x03;
	switch(tone_state){
		case tone_init:
			tone_state = tone_1;
		break;
		
		case tone_1:
			if(tmpA == 0x01){
				tone_state = tone_1;
			}else if(tmpA == 0x02){
				tone_state = tone_2;
			}else{
				tone_state = tone_1;
			}
		break;
		
		case tone_2:
			if(tmpA == 0x01){
				tone_state = tone_1;
			}else if(tmpA == 0x02){
				tone_state = tone_3;
			}else{
				tone_state = tone_2;
			}
		break;
		
		case tone_3:
			if(tmpA == 0x01){
				tone_state = tone_2;
			}else if(tmpA == 0x02){
				tone_state = tone_4;
			}else{
				tone_state = tone_3;
			}
		break;
		
		case tone_4:
			if(tmpA == 0x01){
				tone_state = tone_3;
			}else if(tmpA == 0x02){
				tone_state = tone_5;
			}else{
				tone_state = tone_4;
			}
		break;
		
		case tone_5:
			if(tmpA == 0x01){
				tone_state = tone_4;
			}else if(tmpA == 0x02){
				tone_state = tone_6;
			}else{
				tone_state = tone_5;
			}
		break;
		
		case tone_6:
			if(tmpA == 0x01){
				tone_state = tone_5;
			}else if(tmpA == 0x02){
				tone_state = tone_7;
			}else{
				tone_state = tone_6;
			}
		break;
		
		case tone_7:
			if(tmpA == 0x01){
				tone_state = tone_6;
			}else if(tmpA == 0x02){
				tone_state = tone_8;
			}else{
				tone_state = tone_7;
			}
		break;
		
		case tone_8:
			if(tmpA == 0x01){
				tone_state = tone_7;
				}else if(tmpA == 0x02){
				tone_state = tone_8;
				}else{
				tone_state = tone_8;
			}
		break;
		
		default:
			tone_state = tone_1;
		break;
	}
	switch(tone_state){
		case tone_init:
		break;
		
		case tone_1:
			set_PWM(261.63);
		break;
		
		case tone_2:
			set_PWM(293.66);
		break;
		
		case tone_3:
			set_PWM(329.63);
		break;
		
		case tone_4:
			set_PWM(349.23);
		break;
		
		case tone_5:
			set_PWM(392.00);
		break;
		
		case tone_6:
			set_PWM(440.00);
		break;
				
		case tone_7:
			set_PWM(493.88);
		break;
		
		case tone_8:
			set_PWM(523.25);
		break;
		
		default:
		break;
	}
	
}

unsigned char isON;
enum on_off{on_1, on_2, off_1, off_2} power;
	
void syncSM2(){
	isON = PINA & 0x04;
	switch(power){
		case on_1:
			if(isON == 0){
				power = on_2;
			}else{
				power = on_1;
			}
		break;
		case on_2:
			if(isON != 0){
				power = off_1;
			}else{
				power = on_2;
			}
		break;
		
		case off_1:
			if(isON == 0){
				power = off_2;
			}else{
				power = off_1;
			}
		break;
		
		case off_2:
			if(isON != 0){
				power = on_1;
			}else{
				power = off_2;
			}
		break;
		
		default:
			power = off_2;
		break;
	}
	
	switch(power){
		case on_1:
		break;
		case on_2:
		break;
		
		case off_1:
		set_PWM(0);
		break;
		
		case off_2:
		set_PWM(0);
		break;
		
		default:
		set_PWM(0);
		break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
    /* Replace with your application code */
	tone_state = tone_init;
	PWM_on();
	TimerSet(225);
	TimerOn();
    while(1){
		syncSM1();
		syncSM2();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

