/*
 * omore005_lab03_part2.c
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
 * Prompt: 
 *	A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). A series of
	LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2,
	PC5 lights. If the level is 3 or 4, PC5 and PC4 light. Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2.
	10-12 lights PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should
	light if the level is 4 or less.

 */

int main(void)
{
   
   //initialize Inputs
   DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins 0-3 as inputs
  
   //Initialize Outputs
   DDRC = 0xFF; PORTC = 0x00; //Make PORTC pins as outputs
   
   unsigned char tempA  = 0x00;
   unsigned char tempC  = 0x00;
   
    while (1) 
    {
		tempA  = PINA & 0x0F; //initialize tempA to the value of PINA
		
		//if PINA == some hex, then setbit the needed lights in PINC
		if(tempA == 1 || tempA == 2)
		{
			tempC = 0x60;
		}else if(tempA == 3 || tempA == 4){
			tempC = 0x70;
		}else if(tempA == 5 ||  tempA == 6){
			tempC = 0x38;
		}else if(tempA == 7 || tempA == 8 || tempA == 9){
			tempC = 0x3C;
		}else if(tempA == 10 || tempA == 11 || tempA == 12){
			tempC = 0x3E;
		}else if(tempA == 13 || tempA == 14 || tempA == 15){
			tempC = 0x3F;
		}else{
			tempC = 0x00;	
		}
    
	PORTC = tempC;
	
	}//end of the while
}

