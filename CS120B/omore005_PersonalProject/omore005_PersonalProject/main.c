/*
 * omore005_PersonalProject.c
 *
 * Created: 5/27/2019 5:40:19 PM
 * Author : Osvaldo Moreno Ornelas
 */ 

#include <avr/io.h>
#include "pitches.h"
#include "themes.h"

#define LED_Direction DDRA		/* define LED Direction */
#define LED_PORT PORTA			/* define LED port */

int main(void)
{
    LED_Direction |= 0xff;		/* define LED port direction is output */
    LED_PORT = 0xff;
    
    char array[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
    /* write hex value for CA display from 0 to 9 */
    
    while(1)
    {
	    for(int i=0;i<10;i++)
	    {
		    LED_PORT = array[i]; /* write data on to the LED port */
		    //_delay_ms(1000); /* wait for 1 second */
	    }
    }
    
}

