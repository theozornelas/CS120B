/*
 * omore005_lab2_part1.c
 *
 * Created: 4/4/2019 4:28:04 PM
 * Author : Osvaldo Moreno Ornelas
 * SID:     862016789
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code 
   DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
   DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
   unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
   unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
   while (1)
   {
       // 1) Read input
       tempA = PINA & 0x01;
       // 2) Perform computation
       // if PA0 is 1, set PB1PB0 = 01, else = 10
       if (tmpA == 0x01) { // True if PA0 is 1
           tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbb01
           // (clear rightmost 2 bits, then set to 01)
       }
       else {
           tmpB = (tmpB & 0xFC) | 0x02; // Sets tmpB to bbbbbb10
           // (clear rightmost 2 bits, then set to 10)
       }
       // 3) Write output
       PORTB = tmpB;
   }
   return 0;*/

DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
unsigned char garageDoor = 0x00; // Temporary variable to hold the value of B
unsigned char lightSensor = 0x00; // Temporary variable to hold the value of A
unsigned char ledLight = 0x00;
while (1)
{
// 1) Read input
   garageDoor = PINA & 0x01;
   lightSensor = PINA & 0x02;
//output
   ledLight = PINB & 00;

if(garageDoor == 0x01 && lightSensor == 0x00)
{
	ledLight = (ledLight & 0xFC) | 0x01;

}
else 
{
	ledLight = (ledLight & 0xFC) | 0x00;
	
}

	PORTB = ledLight; //Write output to port
	
}//end of the while
return 0;

}


