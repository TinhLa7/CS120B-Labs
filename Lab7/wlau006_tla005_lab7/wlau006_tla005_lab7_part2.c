/*	Partner 1 Name & E-mail: Wyland Lau, wlau006@ucr.edu
 *	Partner 2 Name & E-mail: Tinh La, tla005@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 7  Exercise 2
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

unsigned char tmpA = 0x00;
unsigned char tmpC = 0x05;
unsigned char tmpB = 0x00;
unsigned char counter = 0x00;
unsigned char game_speeds[15] = {1,1,2,2,3,1,3,2,2,3,4,3,4,4,4};

enum Game_States {Game_init, Game_begin, Game_play, Game_over, Game_s0, Game_s1, Game_s2, Game_s3, Game_victory} Game_State;

void syncSM()
{
	tmpA = PINA & 0x01;
	switch(Game_State) {   // Transitions
		case Game_init:  // Initial transition
		if(tmpA == 1){
			Game_State = Game_begin;
			}else{
			Game_State = Game_init;
		}
		break;
		
		case Game_begin:
		Game_State = Game_play;
		break;

		case Game_play:
		if(tmpC == 9){
			Game_State = Game_victory;
			}else if(counter == 15){
			Game_State = Game_over;
			}else if(game_speeds[counter] == 1){
			Game_State = Game_s0;
			}else if(game_speeds[counter] == 2){
			Game_State = Game_s1;
			}else if(game_speeds[counter] == 3){
			Game_State = Game_s2;
			}else if(game_speeds[counter] == 4){
			Game_State = Game_s3;
		}
		break;
		
		case Game_s0:
		counter++;
		Game_State = Game_play;
		break;
		
		case Game_s1:
		counter++;
		Game_State = Game_play;
		break;
		
		case Game_s2:
		counter++;
		Game_State = Game_play;
		break;
		case Game_s3:
		counter++;
		Game_State = Game_play;
		break;
		
		case Game_victory:
		if(tmpA != 0){
			TimerSet(2000);
			TimerOn();
			Game_State = Game_init;
			LCD_ClearScreen();
			LCD_DisplayString(1,"Restarting game");
			while(!TimerFlag);
			TimerOff();
			TimerFlag = 0;
			}else{
			Game_State = Game_victory;
		}
		break;
		
		case Game_over:
		if(tmpA != 0){
			TimerSet(2000);
			TimerOn();
			LCD_ClearScreen();
			LCD_DisplayString(1,"Restarting game");
			Game_State = Game_init;
			while(!TimerFlag);
			TimerOff();
			TimerFlag = 0;
			}else{
			Game_State = Game_over;
		}
		break;
		
		default:
		Game_State = Game_init;
		break;
	} // Transitions

	switch(Game_State) {   // State actions
		case Game_init:
		LCD_ClearScreen();
		LCD_DisplayString(1,"Press to start");
		break;
		
		//===============================================================================================================================================
		
		case Game_begin:
		TimerSet(2000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"Get to 9");
		while(!TimerFlag);
		TimerFlag = 0;
		LCD_DisplayString(1,"before you run");
		while(!TimerFlag);
		TimerFlag = 0;
		LCD_DisplayString(1, "out of time.");
		while (!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		TimerSet(1000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"Starting in: 3");
		while(!TimerFlag);
		TimerFlag = 0;
		LCD_ClearScreen();
		LCD_DisplayString(1,"Starting in: 2");
		while(!TimerFlag);
		TimerFlag = 0;
		LCD_ClearScreen();
		LCD_DisplayString(1,"Starting in: 1");
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_s0:
		LCD_ClearScreen();
		if(tmpA){
			LCD_DisplayString(1, "Release button");
		}
		
		while(tmpA){
			tmpA = PINA & 0x01;
			PORTB = 0x0F;
		}
		PORTB = 0x00;
		LCD_ClearScreen();
		LCD_DisplayString(1, "Get Ready...");
		TimerSet(4200);
		TimerOn();
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC > 0){
					tmpC -= 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Early!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		TimerSet(600);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"NOW!!!");
		PORTB = 0x0F;
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC < 9){
					tmpC += 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Nice Job!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				PORTB = 0x00;
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		
		PORTB = 0x00;
		TimerSet(2000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1, "Too Slow!");
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_s1:
		LCD_ClearScreen();
		if(tmpA){
			LCD_DisplayString(1, "Release button");
		}
		
		while(tmpA){
			tmpA = PINA & 0x01;
			PORTB = 0x0F;
		}
		PORTB = 0x00;
		LCD_ClearScreen();
		LCD_DisplayString(1, "Get Ready...");
		TimerSet(3300);
		TimerOn();
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC > 0){
					tmpC -= 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Early!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		TimerSet(500);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"NOW!!!");
		PORTB = 0x0F;
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC < 9){
					tmpC += 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Nice Job!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				PORTB = 0x00;
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		
		PORTB = 0x00;
		TimerSet(2000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1, "Too Slow!");
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_s2:
		LCD_ClearScreen();
		if(tmpA){
			LCD_DisplayString(1, "Release button");
		}

		while(tmpA){
			tmpA = PINA & 0x01;
			PORTB = 0x0F;
		}
		PORTB = 0x00;
		LCD_ClearScreen();
		LCD_DisplayString(1, "Get Ready...");
		TimerSet(2500);
		TimerOn();
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC > 0){
					tmpC -= 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Early!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		TimerSet(400);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"NOW!!!");
		PORTB = 0x0F;
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC < 9){
					tmpC += 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Nice Job!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				PORTB = 0x00;
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();

		PORTB = 0x00;
		TimerSet(2000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1, "Too Slow!");
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_s3:
		LCD_ClearScreen();
		if(tmpA){
			LCD_DisplayString(1, "Release button");
		}

		while(tmpA){
			tmpA = PINA & 0x01;
			PORTB = 0x0F;
		}
		PORTB = 0x00;
		LCD_ClearScreen();
		LCD_DisplayString(1, "Get Ready...");
		TimerSet(1200);
		TimerOn();
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC > 0){
					tmpC -= 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Early!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		TimerSet(300);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"NOW!!!");
		PORTB = 0x0F;
		while(!TimerFlag){
			tmpA = PINA & 0x01;
			if (tmpA){
				if(tmpC < 9){
					tmpC += 1;
				}
				TimerOff();
				TimerSet(2000);
				TimerOn();
				LCD_ClearScreen();
				LCD_DisplayString(1, "Nice Job!");
				while(!TimerFlag);
				TimerFlag = 0;
				TimerOff();
				PORTB = 0x00;
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();

		PORTB = 0x00;
		TimerSet(2000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1, "Too Slow!");
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_play:
		LCD_ClearScreen();
		LCD_DisplayString(1, "Current Score:");
		TimerSet(2500);
		TimerOn();
		while(!TimerFlag);
		TimerFlag = 0;
		LCD_WriteData(tmpC + '0');
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_victory:
		LCD_ClearScreen();
		LCD_DisplayString(1,"You win!!");
		TimerSet(500);
		TimerOn();
		PORTB = 0x01;
		while(!TimerFlag){
			tmpA = PINA;
			if(tmpA){
				PORTB = 0x00;
				return;
			}
		}
		TimerFlag = 0;
		PORTB = 0x02;
		while(!TimerFlag){
			tmpA = PINA;
			if(tmpA){
				TimerOff();
				PORTB = 0x00;
				return;
			}
		}
		TimerFlag = 0;
		PORTB = 0x03;
		while(!TimerFlag){
			tmpA = PINA;
			if(tmpA){
				PORTB = 0x00;
				TimerOff();
				return;
			}
		}
		TimerFlag = 0;
		PORTB = 0x04;
		while(!TimerFlag){
			tmpA = PINA;
			if(tmpA){
				PORTB = 0x00;
				TimerOff();
				return;
			}
		}
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		case Game_over:
		TimerSet(5000);
		TimerOn();
		LCD_ClearScreen();
		LCD_DisplayString(1,"You lose...");
		while(!TimerFlag);
		TimerFlag = 0;
		TimerOff();
		break;
		
		//===============================================================================================================================================
		
		default:
		break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF;// Configure port A's 8 pins as inputs, initialize to 
	DDRB = 0xFF; PORTB = 0x00; //LEDs
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
		
		// Initializes the LCD display
		LCD_init();
		Game_State = Game_init;
		while (1)
		{
			// User code (i.e. synchSM calls)
			syncSM();
		}
}
 