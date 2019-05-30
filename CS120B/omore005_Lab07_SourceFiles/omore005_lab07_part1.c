/*
 * omore005_lab07_part1.c
 *
 * Created: 4/26/2019 2:58:59 PM
 * Author : omore005 Osvaldo Moreno Ornelas
 * SID    : 862016789
 *
 * I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work
 
 */ 

#include <avr/io.h>
#include "io.c"

/*Description:
 *Buttons are connected to PA0 and PA1. Output PORTC and PORTD drive the LCD
 display, initially displaying 0. Pressing PA0 increments the display (stopping at 9).
 Pressing PA1 decrements the display (stopping at 0). If both buttons are depressed
 (even if not initially simultaneously), the display resets to 0. If a button is held, then the
 display continues to increment (or decrement) at a rate of once per second. Use a
 synchronous state machine captured in C.
 */

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


enum States{INIT, START, B1_PRESS, B2_PRESS, RESET_PRESS, DEPRESSED}state;
unsigned char num;  //placeholder for number to display
unsigned char tmpA; //Stores the current input
	
void tick()
{
	//state = ~(PINA);
	//state = PINA;
	
	switch(state) //Transitions
	{
		case INIT:
		num=0;
		state=START;
		break;
		
		case START :
		if ((tmpA ==0x01)&& num<9)
		{
			state=B1_PRESS;
		}
		else if ((tmpA==0x02)&& num>0)
		{
			state=B2_PRESS;
		}
		else if ((tmpA==0x03))
		{
			state=RESET_PRESS;
		}
		else {state=START;}
		break;
		
		case B1_PRESS:
		//state = B1_PRESS;
		//if(PINA == 1)
		//{
		//state = B1_RELEASE;
		//}
		state= (tmpA==0x01) ? B1_PRESS : START;
		//state = START;
		//PINA = 0;
		//	else
		//{
		//state = B1_PRESS;
		//}
		break;
		
		case B2_PRESS:
		//state = B2_PRESS;
		//if(PINA == 2)
		//{
		//state = B2_RELEASE;
		//}
		//	else
		//{
		//state = B2_PRESS;
		//}
		state=(tmpA==0x02) ? B2_PRESS : START;
		break;
		
		case RESET_PRESS:
		state= (tmpA==0x00) ? DEPRESSED : RESET_PRESS;
		break;
		
		case DEPRESSED:
		state = START;
		break;
		
		default:
		state=INIT;
		break;
		
	}
	switch(state) //Actions
	{
		case INIT: break;
		
		case START: break;
		
		case B1_PRESS:
		if (num<9)
		{
			num++;
		}
		break;
		
		case B2_PRESS:
		if (num>0)
		{
			num--;
		}
		break;
		
		case RESET_PRESS: break;
		
		case DEPRESSED: ;//PORTB = 0x00;
		num=0;
		break;
		
	}//end of actions switch
}//end of tick function

int main(void)
{
	//inputs:
	DDRA = 0x00; PORTA = 0xFF; //button input
	
	//outputs:
	
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	// Initializes the LCD display
	LCD_init();
	
	TimerSet(1000);
	TimerOn();
	while(1) 
	{
		tmpA= ~PINA&0x03;
		tick();
		LCD_init();
		LCD_WriteData(num +'0');
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
	}
	
}


