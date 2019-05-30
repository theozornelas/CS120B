/*
 * omore005_lab09_Part1.c
 *
 * Created: 5/3/2019 12:11:39 PM
 * Author : omore005
 * SID    : 862016789
  I acknowledge all content contained herein, excluding template or example
  *	code, is my own original work
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

enum States{INIT, WAIT, C4, D4, E4} state;
unsigned char C4Button = 0;
unsigned char D4Button = 0;
unsigned char E4Button = 0;

void tick()
{
	C4Button = (~PINA) & 0x01;
	D4Button = (~PINA) & 0x02;
	E4Button = (~PINA) & 0x04;
	
	switch(state) //Transitions
	{
		case INIT: state = WAIT;
		break;
		
		case WAIT:
		
		if (C4Button && !D4Button && !E4Button) {
			state = C4;
			PWM_on();
			set_PWM(261.63);
			} else if (D4Button &&!C4Button && !E4Button ) {
			state = D4;
			PWM_on();
			set_PWM(293.66);
			} else if (E4Button && !C4Button && !D4Button ) {
			state = E4;
			PWM_on();
			set_PWM(329.63);
		}
		else 
		{			
			state = WAIT;
		}
		
		break;
		
		case C4:
		if (C4Button && !D4Button && !E4Button) {
			state = C4;
		} 
		else
		{
			state = WAIT;
		}
		break;
		
		case D4:
		if (D4Button &&!C4Button && !E4Button) {
			state = D4;
			} else {
			state = WAIT;
		}
		break;
		
		case E4:
		if (E4Button && !C4Button && !D4Button) {
			state = E4;
			} else {
			state = WAIT;
		}
		break;
		
	}
	switch(state) //Actions
	{
		case INIT:
		break;
		
		case WAIT:
			PWM_off();
		break;
		
		case C4:
		break;
		
		case D4:
		break;
		
		case E4:
		break;
		
	}//end of actions switch
}//end of tick function


int main(void)
{
    DDRA = 0x00; PORTA  = 0xFF; //set for input 0-3 for button
	DDRB = 0xFF; PORTB = 0x00; //set for output 0 for speaker
    while (1) 
    {
		tick();
		
    }
}

