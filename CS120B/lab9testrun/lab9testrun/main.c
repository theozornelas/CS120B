#include <avr/io.h>

enum SM_Stats {SM_Start, SM_wait, SM_C4, SM_D4, SM_E4} mySM;
unsigned char C4_button = 0;
unsigned char D4_button = 0;
unsigned char E4_button = 0;


// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {

	static double current_frequency = -1; // Keeps track of the currently set frequency

	// Will only update the registers when the frequency changes, otherwise allows

	// music to play uninterrupted.

	if (frequency != current_frequency) {

		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter

		else { TCCR3B |= 0x03; } // resumes/continues timer/counter

		

		// prevents OCR3A from overflowing, using prescaler 64

		// 0.954 is smallest frequency that will not result in overflow

		if (frequency < 0.954) { OCR3A = 0xFFFF; }

		

		// prevents OCR0A from underflowing, using prescaler 64     // 31250 is largest frequency that will not result in underflow

		else if (frequency > 31250) { OCR3A = 0x0000; }

		

		// set OCR3A based on desired frequency

		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		

		TCNT3 = 0; // resets counter

		current_frequency = frequency; // Updates the current frequency

	}

}



void PWM_on() {

	TCCR3A = (1 << COM3A0);

	// COM3A0: Toggle PB3 on compare match between counter and OCR0A

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	// WGM02: When counter (TCNT0) matches OCR0A, reset counter

	// CS01 & CS30: Set a prescaler of 64

	set_PWM(0);

}



void PWM_off() {

	TCCR3A = 0x00;

	TCCR3B = 0x00;

}











void tickFct() {
	C4_button = (~PINA) & 0x01;
	D4_button = (~PINA) & 0x02;
	E4_button = (~PINA) & 0x04;
	
	//C4_button = (PINA) & 0x01;
	//D4_button = (PINA) & 0x02;
	//E4_button = (PINA) & 0x04;
	
	switch (mySM) {
		case SM_Start:
		mySM = SM_wait;
		break;
		case SM_wait:
		if (C4_button && !D4_button && !E4_button) {
			mySM = SM_C4;
			PWM_on();
			set_PWM(261.63);
			} else if (D4_button &&!C4_button && !E4_button ) {
			mySM = SM_D4;
			PWM_on();
			set_PWM(293.66);
			} else if (E4_button && !C4_button && !D4_button ) {
			mySM = SM_E4;
			PWM_on();
			set_PWM(329.63);
		}
		else mySM = SM_wait;
		//PWM_on();
		//set_PWM(261.63); //DO NOT LEAVE IN

		break;
		case SM_C4:
		if (C4_button && !D4_button && !E4_button) {
			mySM = SM_C4;
			} else {
			mySM = SM_wait;
		}
		break;
		case SM_D4:
		if (D4_button &&!C4_button && !E4_button) {
			mySM = SM_D4;
			} else {
			mySM = SM_wait;
		}
		break;
		case SM_E4:
		if (E4_button && !C4_button && !D4_button) {
			mySM = SM_E4;
			} else {
			mySM = SM_wait;
		}
		break;
		default:
		mySM = SM_wait;
		break;
	}
	switch (mySM) {
		case SM_Start:
		//do nothing
		break;
		case SM_wait:
		PWM_off();
		break;
		case SM_C4:
		//PWM_on();
		//set_PWM(261.63);
		break;
		case SM_D4:
		//PWM_on();
		//set_PWM(293.66);
		break;
		case SM_E4:
		//PWM_on();
		//set_PWM(329.63);
		break;

	}
}






int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; //lab said DDRB = xxxx 1xxx
	/* Replace with your application code*/
	//PWM_on();
	//set_PWM(261.63);
	mySM = 0;
	while (1)
	{
		tickFct();
	}
}