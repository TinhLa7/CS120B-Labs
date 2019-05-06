/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 9  Exercise 1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}
unsigned char tmpA;
enum tone_states{ tone_init, tone_1, tone_2, tone_3, tone_wait} tone_state;
void syncSM(){
	tmpA = PINA & 0x07;
	switch(tone_state){
		case tone_init:
			tone_state = tone_wait;
		break;
		
		case tone_wait:
			if(tmpA == 0x01){
				tone_state = tone_1;
			}else if(tmpA == 0x02){
				tone_state = tone_2;
			}else if(tmpA == 0x04){
				tone_state = tone_3;
			}else{
				tone_state = tone_wait;
			}
		break;
		
		case tone_1:
			if(tmpA != 0x01){
				tone_state = tone_wait;
			}else{
				tone_state = tone_1;
			}
		break;
		
		case tone_2:
			if(tmpA != 0x02){
				tone_state = tone_wait;
			}else{
				tone_state = tone_2;
			}
		break;
		
		case tone_3:
			if(tmpA != 0x04){
				tone_state = tone_wait;
			}else{
				tone_state = tone_3;
			}
		break;
		
		case default:
			tone_state = tone_wait;
		break;
	}
	switch(tone_state){
		case tone_init:
		break;
		
		case tone_wait:
			set_PWM(0);
			PWM_off();
		break;
		
		case tone_1:
			set_PWM(261.63);
			PWM_on();
		break;
		
		case tone_2:
			set_PWM(293.66);
			PWM_on();
		break;
		
		case tone_3:
			set_PWM(329.63);
			PWM_on();
		break;
		
		case default:
			tone_state = tone_wait;
		break;
	}
	
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
    /* Replace with your application code */
	tone_state = tone_init;
    while (1) 
    {
		syncSM();
    }
}

