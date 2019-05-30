/*
 * omore005_lab08_part1.c
 *
 * Created: 5/1/2019 12:45:58 PM
 * Author : omore005
 * SID    : 862016789
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
   
    while (1) 
    {
		 x = ADC; // Value of ADC register now stored in variable x.
		my_char = (char)x; // my_char = 0xCD
		PORTB = my_char;
		my_char = (char)(x >> 8); // my_char = 0xBC		PORTD = my_char;
    }//end of while loop
}

