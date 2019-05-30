/*
 * omore005_PersonalProject.c
 *
 * Created: 5/27/2019 5:40:19 PM
 * Author : Osvaldo Moreno Ornelas
 */ 

#include <avr/io.h>
#include "pitches.h"
#include "themes.h"

void set_PWM(double frequency) {

	static double current_frequency = -1; // Keeps track of the currently set frequency

	// Will only update the registers when the frequency changes, otherwise allows

	// music to play uninterrupted.

	if (frequency != current_frequency) {

		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter

		else { TCCR3B |= 0x03; } // resumes/continues timer/counter

		

		// prevents OCR3A from overflowing, using prescaler 64

		// 0.954 is smallest frequency that will not result in overflow

		if (frequency < 0.954) { OCR3A = 0xFFFF; }

		

		// prevents OCR0A from underflowing, using prescaler 64     // 31250 is largest frequency that will not result in underflow

		else if (frequency > 31250) { OCR3A = 0x0000; }

		

		// set OCR3A based on desired frequency

		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		

		TCNT3 = 0; // resets counter

		current_frequency = frequency; // Updates the current frequency

	}

}



void PWM_on() {

	TCCR3A = (1 << COM3A0);

	// COM3A0: Toggle PB3 on compare match between counter and OCR0A

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	// WGM02: When counter (TCNT0) matches OCR0A, reset counter

	// CS01 & CS30: Set a prescaler of 64

	set_PWM(0);

}



void PWM_off() {

	TCCR3A = 0x00;

	TCCR3B = 0x00;

}volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

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
}unsigned int notes[] = {262, 262, 294, 262, 349, 330, 262, 262, 294, 262, 392,

/*                       you, Hap py  Birth Day  dear  xxxx      Hap  py   birth
                         F4   C4   C4   C5   A4   F4   E4   D4   B4b  B4b  A4 */
                        349, 262, 262, 523, 440, 349, 330, 294, 466, 466, 440,

/*                       day  to  you
                         F4   G4   F4   */
                        349, 392, 349
                        };						const double notes2[26] = {329.63, 329.63, 329.63, 329.63, 329.63, 329.63, 329.63, 392.00, 261.63, 293.66, 329.63, 349.23, 349.23, 349.23, 349.23, 349.23, 329.63, 329.63, 329.63, 329.63, 329.63, 293.66, 293.66, 329.63, 293.66, 392.00 };
						unsigned short interval[] = {4, 4, 8, 8, 8, 10, 4, 4, 8, 8, 8, 10, 4, 4, 8, 8, 8,
8, 8, 4, 4, 8, 8, 8, 12};
enum SM_Stats {SM_Start, SM_wait, MELODY} mySM;
int i = 0;
void tickFct() {
	
	switch (mySM) {
		case SM_Start:
		mySM = SM_wait;
		break;
		case SM_wait:
		mySM = MELODY;
		break;
		case MELODY:
		PWM_on();
		set_PWM(notes[i]);
		if(i<25)
		{
			i++;
		}
		break;
		
		default:
		mySM = SM_wait;
		break;
	}
	switch (mySM) {
		case SM_Start:
		//do nothing
		break;
		case SM_wait:
		PWM_off();
		break;
		case MELODY:
		
		//set_PWM(0);
		//TimerSet(interval[i]);
		//TimerOn();
		
		
		break;

	}
}






int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; //lab said DDRB = xxxx 1xxx
	/* Replace with your application code*/
	//PWM_on();
	//set_PWM(261.63);
	
	//TimerSet(400);
	//TimerOn();
	//PWM_on();
	mySM = 0;
	while (1)
	{
		tickFct();
		//while(!TimerFlag){}
		//TimerFlag = 0;
	}
}