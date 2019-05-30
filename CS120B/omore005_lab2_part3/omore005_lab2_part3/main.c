/*
 * omore005_lab02_part2.c
 *
 * Created: 4/5/2019 1:13:11 PM
 * Author : Osvaldo Moreno Ornelas 862016789
 */ 

#include <avr/io.h>


int main(void)
{	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins 0-3 as inputs
	DDRC = 0xFF; PORTC = 0x00; //Make PORTC pins as outputs

	unsigned char tempC0 = 0x00;
	unsigned char tempC1 = 0x00;
	unsigned char tempC2 = 0x00;
	unsigned char tempC3 = 0x00;
	unsigned char tempC7 = 0x00;

    unsigned char cntavail = 0x04;
   
    while (1) 
    {
		tempC0 = PINA & 0x01;
		tempC1 = PINA & 0x03;
		tempC2 = PINA & 0x07;
		tempC3 = PINA & 0x0F;

		if (tempC0 == 0x01)
		{
			cntavail = cntavail - 1;
		}
		if (tempC1 == 0x03)
		{
    		cntavail = cntavail - 1;
		}
		if (tempC2 == 0x07)
		{
    		cntavail = cntavail - 1;
		}
		if (tempC3 == 0x0F)
		{
    		cntavail = cntavail - 1;
		}

		if(cntavail == 0x00)
		{
    		//cntavail = tempC7 & 0x80;
			tempC7 = 0x80;
			PORTC = tempC7;
		}

		//PORTC = cntavail;
		cntavail = 0x04;
    }
}

