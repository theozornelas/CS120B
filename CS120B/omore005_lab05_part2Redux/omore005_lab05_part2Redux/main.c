/*
 * omore005_lab05_part2Redux.c
 *
 * Created: 4/22/2019 4:47:27 PM
 * Author : osvaldo moreno ornelas omore005
 * SID    : 862016789
 */ 

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum States{START,B1_PRESS,B1_RELEASE, B2_PRESS, B2_RELEASE ,DEPRESSED}state;
	
/*
 * Prompt: 
 *	2. Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0
 * increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once
 * (stopping at 0). If both buttons are depressed (even if not initially simultaneously),
 * PORTC resets to 0.
 */	
	
void tick()
{
	//state = ~(PINA);
	state = PINA;
	
	switch(state) //Transitions
	{
		case 0x03 : 
		//state = DEPRESSED;
		break;
	
		case B1_PRESS: 
		//state = B1_PRESS;	
			//if(PINA == 1)
			//{
				//state = B1_RELEASE;
			//}
			
			PORTB++;
			state = START;
			PINA = 0;
		//	else
			//{
				//state = B1_PRESS;
			//}	
		break;
		
		case 0x02:
		//state = B2_PRESS;
		//if(PINA == 2)
		//{
			//state = B2_RELEASE;
		//}
	//	else
		//{
			//state = B2_PRESS;
		//} 
		break;
		
	}
	switch(state) //Actions
	{
		case START: break;
		
		case B1_PRESS: 
		break;
		
		case B2_PRESS: 
		break;	
		
		case B1_RELEASE:
			if(PORTB < 9)
				PORTB++;
		//PINA = 0;
		break;
		
		case B2_RELEASE:
			//if(PORTB != 0)
				//PORTB--;
		//PINA = 0;
		break;
		
		case DEPRESSED: ;//PORTB = 0x00;
		break;
		
	}
	
	
}



int main(void)
{
	//initialize Inputs
	DDRA = 0x00; PORTA = 0x0F; // Configure port A's pins 0-3 as button input
	//state = ~(PINA);
	
	DDRB = 0xFF; PORTB = 0x00; // start port C with value of 0

    while (1) 
    {	
		tick();
    }
}

