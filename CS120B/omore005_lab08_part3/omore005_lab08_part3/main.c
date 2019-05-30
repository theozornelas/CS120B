/*
 * omore005_lab08_part3.c
 *
 * Created: 5/2/2019 4:27:38 PM
 * Author : omore005
 *  I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work
 */ 

#include <avr/io.h>


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
	unsigned short x = 0x00;
	unsigned short my_char = 0x00;
	unsigned short MAX = 200;
	
	while (1)
	{
		x = ADC; // Value of ADC register now stored in variable x.
		
		if(x > (MAX >> 1))
		{
			my_char = 0xFF;
		}
		else
		{
			my_char = 0x00;
		}
				PORTB = my_char;
		//my_char = (char)(x >> 8); // my_char = 0xBC		//PORTD = my_char;		
	}//end of while loop
}