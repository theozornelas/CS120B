/*
 * omore005_lab09_Part2.c
 *
 * Created: 5/3/2019 12:11:39 PM
 * Author : omore005 osvaldo moreno ornelas
 * SID    : 862016789
 *  I acknowledge all content contained herein, excluding template or example
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

enum States {INIT, C4, D4, E4, F4, G4, A4, B4, C5, UP, DOWN} state;
enum Toggle {WAIT, TOGGLE} toggle;

unsigned char prev    = 0; //keeps track of previuos note
unsigned char down    = 0;
unsigned char up      = 0;
unsigned char pressed = 0;

void tick()
{	
	up = (~PINA) & 0x02;//second goes up
	down = (~PINA) & 0x01;//first sets down
		
	switch(state) //Transitions
	{
		case INIT: state = C4;
		break;
		
		case C4:
		if (down) {
			state = C4;
		} 
		else
		{
			prev = state;
			state = UP;
		}
		break;
		
		case D4:
		if (down) {
			prev = state;
			state = DOWN;
		} 
		else if (up) 
		{
			prev = state;
			state = UP;
		}
		break;
		
		case E4:
		if (down) {
			prev = state;
			state = DOWN;
		} 
		else if (up) 
		{
			prev = state;
			state = UP;
		}
		break;
		
		
		case F4:
		if (down) 
		{
			prev = state;
			state = DOWN;
		}
		else if (up)
		{
			prev = state;
			state = UP;
		}
		break;
		
		case G4:
		if (down) 
		{
			prev = state;
			state = DOWN;
		}
		else if (up)
		{
			prev = state;
			state = UP;
		}
		break;
		
		case A4:
		if (down) 
		{
			prev = state;
			state = DOWN;
		}
		else if (up)
		{
			prev = state;
			state = UP;
		}
		break;
		
		case B4:
		if (down) 
		{
			prev = state;
			state = DOWN;
		}
		else if (up)
		{
			prev = state;
			state = UP;
		}
		break;
		case C5:
		if (down) 
		{
			prev = state;
			state = DOWN;
		} 
		else if (up) 
		{
			state = C5;//loop back
		}
		break;
			
		case UP: 
			if(up)
			{
				state = UP;
			}
			else
			{
				prev++;
			}
		break;
		case DOWN: state = down ? DOWN : prev--;
			if(down)
			{
				state = DOWN;
			}
			else
			{
				prev--;
			}
		break;
		
	}
	switch(state) //Actions
	{
		//set frequencies
		case C4:
			set_PWM(261.63);
		break;
		case D4:
			set_PWM(293.66);
		break;
		case E4:
			set_PWM(329.63);
		break;
		case F4:
			set_PWM(349.23);
		break;
		case G4:
			set_PWM(392.00);
		break;
		case A4:
			set_PWM(440.00);
		break;
		case B4:
			set_PWM(493.88);
		break;
		case C5:
			set_PWM(523.25);
		break;
		
	}//end of actions switch
}//end of tick function

//second tick to control the holding down while pressing
tick2() {
	pressed = (~PINA) & 0x01;
	switch (toggle) {
		case WAIT:
			if (pressed) {
				pressed = TOGGLE;
			}
		break;
		
		case TOGGLE:
			toggle = pressed ? toggle : WAIT;
		break;

	}//end of transitions
	
	switch (toggle) {
		case WAIT:
		break;
		case TOGGLE:
		//needs work
		break;
	}
}

int main(void)
{
    DDRA = 0x00; PORTA  = 0xFF; //set for input 0-3 for button
	DDRB = 0xFF; PORTB = 0x00; //set for output 0 for speaker
    while (1) 
    {
		//use the state machines
		tick();
		tick2();
		
    }
}

