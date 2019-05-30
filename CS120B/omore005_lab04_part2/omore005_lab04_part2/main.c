/*
 * omore005_lab04_part2.c
 *
 * Created: 4/11/2019 5:57:49 PM
 * Author : omore005 
 * EMAIL  : omore005@ucr.edu
 * SID    : 862016789
 * I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>


// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum States{START,B1_PRESS, B2_PRESS, DEPRESSED}state;
	
/*
 * Prompt: 
 *	2. Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0
 * increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once
 * (stopping at 0). If both buttons are depressed (even if not initially simultaneously),
 * PORTC resets to 0.
 */	


	
void tick()
{
	
	//DDRC = 0xFF; PORTC = 0x00; // Configure port B's pins 0-3 as LED output
	
	switch(state) //Transitions
	{
		case START : 
			//if(PINA == 0x01)
			//{
				//state = B1_PRESS;
			//}else if(PINA == 2)
			//{
				//state = B2_PRESS;
			//}
		break;
	
		case B1_PRESS: 		
		break;
		
		case B2_PRESS: 
		break;
		
		case DEPRESSED: PORTB = 0;
		break;
		
	}
	switch(state) //Actions
	{
		case START: break;
		
		case B1_PRESS: 
			if(PORTB < 9)
				PORTB++;
			//state = START;
		break;
		
		case B2_PRESS: 
			if(PORTB != 0)
				PORTB--;
			state = START;
		break;	
		
		case DEPRESSED: ;//state = START;;
		break;
		
	}
	
	
}



int main(void)
{
	//initialize Inputs
	DDRA = 0x00; PORTA = 0x0F; // Configure port A's pins 0-3 as button input
	state = ~(PINA);
	
	DDRB = 0xFF; PORTB = 0x00; // start port C with value of 7

    while (1) 
    {	
		tick();
    }
}

