#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char soundOutput = 0x00;

unsigned char tmpA = 0x00;
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

enum threeStates{threeInit,first, second, third}threeState;
void SM_Three_Tick(){
	switch(threeState){ // Transitions
		case threeInit:
		threeState = first;
		break;
		case first:
		threeState = second;
		break;
		case second:
		threeState = third;
		break;
		case third:
		threeState = first;
		break;
	}
	switch(threeState){ // State actions
		case threeInit:
		break;
		case first:
		threeLEDs = 0x01;
		break;
		case second:
		threeLEDs = 0x02;
		break;
		case third:
		threeLEDs = 0x04;
		break;
	}
}

enum singleStates{on, off}singleState;
void SM_Single_Tick(){
	switch(singleState){ // Transitions
		case on:
		singleState = off;
		break;
		case off:
		singleState = on;
		break;
	}
	switch(singleState){ // State Actions
		case on:
		blinkingLED = 0x08;
		break;
		case off:
		blinkingLED = 0x00;
		break;

	}
}
enum opStates{output} opState;
void SM_output(){
	switch(opState){ // Transitions
		case output:
		break;
	}
	switch(opState){ // State actions
		case output:
		tmpB = threeLEDs | blinkingLED | soundOutput;
		break;
	}

}
enum bStates{wait, soundOn, soundOff} bState;
void SM_sound(){
	tmpA = ~PINA & 0x04;
	switch(bState){ // Transitions
		case wait:
		if(tmpA){
			bState = soundOn;
		}
		else{
			bState = wait;
		}
		break;
		case soundOn:
		if(tmpA){
			bState = soundOff;
		}
		else{
			bState = wait;
		}
		break;
		case soundOff:
		if(tmpA){
			bState = soundOn;
		}
		else{
			bState = wait;
		}
	}
	switch(bState){ // State actions
		case wait:
		soundOutput = 0x00;
		break;
		case soundOn:
		soundOutput = 0x10;
		break;
		case soundOff:
		soundOutput = 0x00;
		break;
	}
};

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Set A to input and initialize to 0xFF
	DDRB = 0xFF; PORTB = 0x00; // Set B to output and initialize to 0x00
	//DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	//DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	unsigned long lelapsedTime = 0;
	unsigned long selapsedTime = 0;
	unsigned long largePeriod = 1000;
	unsigned long smallPeriod = 300;
	const unsigned long TimerPeriod = 2;
	unsigned short buzzElapsedTime = 0;

	TimerSet(TimerPeriod);
	TimerOn();
	
	opState = output;
	threeState = threeInit;
	singleState = on;
	while(1){
		if(lelapsedTime >= largePeriod){
			SM_Single_Tick();
			lelapsedTime = 0;
		}
		if(selapsedTime >= smallPeriod){
			SM_Three_Tick();
			selapsedTime = 0;
		}
		//if (buzzElapsedTime >= 2 && tmpA){
		//	SM_sound();
		//	buzzElapsedTime = 0;
		//}
		SM_sound();
		SM_output();
		PORTB = tmpB;
		while(!TimerFlag){}
		TimerFlag = 0;
		lelapsedTime += TimerPeriod;
		selapsedTime += TimerPeriod;
	}
}