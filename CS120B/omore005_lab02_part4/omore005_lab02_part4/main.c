/*
 * omore005_lab02_part4.c
 *
 * Created: 4/5/2019 4:09:28 PM
 * Author : Osvaldo Moreno Ornelas 862016789
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins 0-3 as inputs

	DDRB = 0x00; PORTB = 0xFF; // Configure port A's pins 0-3 as inputs

	DDRC = 0x00; PORTC = 0xFF; // Configure port A's pins 0-3 as inputs

    DDRD = 0xFF; PORTD = 0x00; //Make PORTC pins as outputs

    unsigned char tempD0 = 0x00;
    unsigned char tempD1 = 0x00;
    unsigned char tempD2 = 0x00;
    unsigned char tempD3 = 0x00;

    unsigned char actualWeight = 0x00;
	unsigned char shiftedWeight = 0x00;

    
    while (1)
    {

		/*tempD0 = PIND & 0x01;
		tempD1 = PIND & 0x03;
		tempD2 = PIND & 0x07;
		tempD3 = PIND & 0x0F;*/
	
		actualWeight = PORTA+PORTB+PORTC;
		
      if(actualWeight > 140)
	  {
		PIND = 0x01;
	  }
	 if((PORTA - PORTC) > 80)
	 {
		PIND = 0x02;

	 }

    }
}

