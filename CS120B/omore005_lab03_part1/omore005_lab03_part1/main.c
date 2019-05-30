/*
 * omore005_lab03_part1.c
 *
 * Created: 4/9/2019 5:30:04 PM
 * Author : omore005
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

/*
 * Prompt: Count the number of 1s on ports A and B 
 *         and output that number on port C.
 */
int main(void)
{
	//initialize Inputs
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins 0-3 as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's pins 0-3 as inputs
	
	//Initialize Outputs
    DDRC = 0xFF; PORTC = 0x00; //Make PORTC pins as outputs
	
	while(1)
	{
		unsigned char bitCounter = 0; //counter for the num of active bits
		
		for(unsigned char i = 0; i<8; ++i)
		{
			//check if ant of the ports have active bits
			if(GetBit(PINA,i) == 1)
			{
				++bitCounter; //if so increase counter
			}
			if(GetBit(PINB,i) == 1)
			{
				++bitCounter;			
			}
		}//end of for
			
		PORTC = bitCounter; //assign value to PORTC
		
	}//end of the while
    
	
}

