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
		
		case default:
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
		
		case default:
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
		
		case default:
			power = off_2;
		break;
	}
	
	switch(power){
		case on_1:
		PWM_on();
		break;
		case on_2:
		PWM_on();
		break;
		
		case off_1:
		PWM_off();
		break;
		
		case off_2:
		PWM_off();
		break;
		
		case default:
		PWM_off();
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
		syncSM1();
		syncSM2();
    }
}

