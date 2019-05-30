/*
 * omore005_lab02_part2.c
 *
 * Created: 4/5/2019 1:13:11 PM
 * Author : Osvaldo Moreno Ornelas
 * SID    : 862016789
 */ 

#include <avr/io.h>


int main(void)
{	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins 0-3 as inputs
	DDRC = 0xFF; PORTC = 0x00; //Make PORTC pins as outputs

	unsigned char tempA0 = 0x00;
	unsigned char tempA1 = 0x00;
	unsigned char tempA2 = 0x00;
	unsigned char tempA3 = 0x00;

    unsigned char cntavail = 0x04;
   
    while (1) 
    {
		tempA0 = PINA & 0x01;
		tempA1 = PINA & 0x03;
		tempA2 = PINA & 0x07;
		tempA3 = PINA & 0x0F;

		if (tempA0 == 0x01)
		{
			cntavail = cntavail - 1;
		}
		if (tempA1 == 0x03)
		{
    		cntavail = cntavail - 1;
		}
		if (tempA2 == 0x07)
		{
    		cntavail = cntavail - 1;
		}
		if (tempA3 == 0x0F)
		{
    		cntavail = cntavail - 1;
		}

		PORTC = cntavail;
		cntavail = 0x04;
    }
}

