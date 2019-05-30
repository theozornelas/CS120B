/*
 * lab9_2.c
 *
 * Created: 5/7/2019 9:49:30 AM
 * Author : osmor
 */ 

#include <avr/io.h>

enum SM_Stats {SM_Start, SM_C4, SM_D4, SM_E4, SM_F4, SM_G4, SM_A4, SM_B4, SM_C5, INC, DEC} mySM;
enum SM_Toggle {SM_wait, SM_toggle} SM_toggleSwitch;
enum bool {false, true} myBool;
unsigned char down_step = 0;
unsigned char up_step = 0;
unsigned char power = 0;

unsigned char prev_state = 0;

void set_PWM(double frequency) {

	static double current_frequency; // Keeps track of the currently set frequency

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
	down_step = (~PINA) & 0x02;
	up_step = (~PINA) & 0x04;
	switch (mySM) {
		case SM_Start:
		mySM = SM_C4;
		break;
		case INC: mySM = up_step ? INC : prev_state + 1;
		break;
		case DEC: mySM = down_step ? DEC : prev_state - 1;
		break;
		case SM_C4:
		if (down_step) {
			mySM = SM_C4;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_D4:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_E4:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_F4:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_G4:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_A4:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_B4:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			prev_state = mySM;
			mySM = INC;
		}
		break;
		case SM_C5:
		if (down_step) {
			prev_state = mySM;
			mySM = DEC;
			} else if (up_step) {
			mySM = SM_C5;
		}
		break;
		default:
		mySM = SM_Start;
	}
	switch (mySM) {
		case SM_C4:
		set_PWM(261.63);
		break;
		case SM_D4:
		set_PWM(293.66);
		break;
		case SM_E4:
		set_PWM(329.63);
		break;
		case SM_F4:
		set_PWM(349.23);
		break;
		case SM_G4:
		set_PWM(392.00);
		break;
		case SM_A4:
		set_PWM(440.00);
		break;
		case SM_B4:
		set_PWM(493.88);
		break;
		case SM_C5:
		set_PWM(523.25);
		break;
	}
}

tickFct_2() {
	power = (~PINA) & 0x01;
	switch (SM_toggleSwitch) {
		case SM_wait:
		if (power) {
			SM_toggleSwitch = SM_toggle;
		}
		break;
		case SM_toggle:
		SM_toggleSwitch = power ? SM_toggleSwitch : SM_wait;
		break;
		default:
		SM_toggleSwitch = SM_wait;
		break;
	}
	switch (SM_toggleSwitch) {
		case SM_wait:
		//do nothing
		break;
		case SM_toggle:
		if (myBool) {
			myBool = false;
			PWM_off();
			} else {
			myBool = true;
			PWM_on();
		}
		break;
	}
}



int main(void)
{
	DDRA = 0x00; PINA = 0xFF;
	DDRB = 0xFF; PINB = 0x00; //lab said DDRB = xxxx 1xxx
	/* Replace with your application code */
	while (1)
	{
		tickFct();	//controls other functionality
		tickFct_2(); //controls on or off
	}
}