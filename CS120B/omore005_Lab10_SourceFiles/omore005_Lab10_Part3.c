/*
 * omore005_Lab10_Part3.c
 *
 * Created: 5/12/2019 10:54:50 PM
 * Author : omore005 Osvaldo Moreno Ornelas
 * SID    : 862016789
 *
 * I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work
 */ 

#include <avr/io.h>

/************************************************************************/
/* Here is the given code from class	                                */
/************************************************************************/

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

/************************************************************************/
/* Here is the start of the code that drives the hardware logic.                                                                     */
/************************************************************************/
unsigned char tempB = 0x00;
unsigned char LEDs = 0x00;
unsigned char blinkLED = 0x00;

enum threeLights{THREE_INIT,FIRST, SECOND, THIRD}threeLight;

//this transitions loop the lights
void Three_LED_Tick(){
	switch(threeLight){ // Transitions
		case THREE_INIT:
		threeLight = FIRST;
		break;
		case FIRST:
		threeLight = SECOND;
		break;
		case SECOND:
		threeLight = THIRD;
		break;
		case THIRD:
		threeLight = FIRST;
		break;
	}
	switch(threeLight){ //actions
		
		case THREE_INIT:
		break;
		case FIRST:
		LEDs = 0x01;
		break;
		case SECOND:
		LEDs = 0x02;
		break;
		case THIRD:
		LEDs = 0x04;
		break;
	}
}

//lits the led on and off
enum oneStates{ON, OFF}oneState;

void ONE_LED_Tick(){
	switch(oneState){ //Transitions
		case ON:
		oneState = OFF;
		break;
		case OFF:
		oneState = ON;
		break;
	}
	switch(oneState){ //Actions
		case ON:
		blinkLED = 0x08;
		break;
		case OFF:
		blinkLED = 0x00;
		break;

	}
}

//checks for what is happening and outputs
//the correct result
enum operations{OUT} operation;

void OUT_Tick(){
	switch(operation){ //Transitions
		case OUT:
		break;
	}
	switch(operation){ //actions
		case OUT:
		tempB = LEDs | blinkLED;
		break;
	}

}

//sound controlling SM
enum Sounds{WAIT, S_ON, S_OFF} sound;
unsigned char tempA; //stores pina input
unsigned char sOutput;
	
void SOUND_Tick(){
	tempA = ~PINA & 0x04;
	switch(sound){ // Transitions
		case WAIT:
		if(tempA){
			sound = S_ON;
		}
		else{
			sound = WAIT;
		}
		break;
		case S_ON:
		if(tempA){
			sound = S_OFF;
		}
		else{
			sound = WAIT;
		}
		break;
		case S_OFF:
		if(tempA){
			sound = S_ON;
		}
		else{
			sound = WAIT;
		}
	}
	switch(sound){ // State actions
		case WAIT:
		sOutput = 0x00;
		break;
		case S_ON:
		sOutput = 0x10;
		break;
		case S_OFF:
		sOutput = 0x00;
		break;
	}
};

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //initialize PORTB for Output
	
	unsigned long period = 1000;
	unsigned long timer = 0;	
	const unsigned long smallPeriod = 300;

	TimerSet(1000);
	TimerOn();
	
	threeLight = THREE_INIT;
	operation  = OUT;
	oneState   = ON;
	
	while(1){
	
	/************************************************************************
	 *STILL UNDER CONSTRUCTION
	/************************************************************************/	
	
	}
}
