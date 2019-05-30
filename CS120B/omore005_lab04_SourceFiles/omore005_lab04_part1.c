/*
 * omore005_lab04_part1.c
 *
 * Created: 4/11/2019 12:52:04 PM
 * Author : omore005
 * email  : omore005@ucr.edu
 * SID    : 862016789
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>

enum States{START,ON_PRESS, ON_RELEASE, OFF_PRESS, OFF_RELEASE}state;
	
/*
 * Prompt: 
 *	1.PB0 and PB1 each connect to an LED, and PB0's LED is initially on. Pressing a button
 connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after
 button release. Pressing the button again turns off PB1's LED and turns on PB0's LED.
 */	


	
void tick()
{
	
	
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's pins 0-3 as LED output
	
	unsigned char tempB  = 0x01;
	
	switch(state) //Transitions
	{
		case START: tempB = 0x01;
					state  = ON_PRESS;
		break;
		
		case ON_PRESS: if(tempB == 1) tempB = 0x02;
		break;
		
		case ON_RELEASE: tempB = 0x02;
		break;
		
		case OFF_PRESS: if(tempB == 2) tempB = 0x01;
		
		break;
		
		case OFF_RELEASE: tempB = 0x01;
			
		break;	
	}
	switch(state) //Actions
	{
		case START: 
		break;
		
		case ON_PRESS:
		break;
		
		case ON_RELEASE: 
		break;
		
		case OFF_PRESS:		
		break;
		
		case OFF_RELEASE: 
		break;
	}
	
	PORTB = tempB;
}



int main(void)
{
	//initialize Inputs
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins 0-3 as button input
	state = PINA;

    while (1) 
    {
		tick();
    }
}

