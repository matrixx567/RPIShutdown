/************************************************************************
Project:  RPISupply
Title:    Add a supply switch to the raspberry pi
Author:   Martin Noehrer
Email:    martin@mnprojects.net
Homepage: https://github.com/matrixx567/RPIShutdown
File:     main.c
Date:

License:  MIT License

LICENSE:
Copyright (C) 2013 Martin Noehrer

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
 *************************************************************************
Software: WIN-AVR 20100110 - AVR-GCC 4.3.3, AVR Libc 1.6.7
Hardware: ATtiny45 at 8Mhz /DIV8 => 1MHz
Fuses:    LFUSE:62 HFUSE:DF EFUSE:FF
Fuses:    LFUSE:62 HFUSE:DD EFUSE:FF	new Setting
Usage:
 *************************************************************************
Pin Description:
B0: inp	Switch		 low...pressed
B1:	out	RPI-TX		 output to raspberrypi
B2:	out	Status-LED   low...LED on
B3:	out	Relay		 high...on
B4:	inp	RPI-RX		 input from raspberrypi
 *************************************************************************
Description: see documentation
 *************************************************************************
Changelog:
 *************************************************************************
contains code and ideas from:
 ************************************************************************/


 // #define USE_MOSFET

//internal 8MHz oscillator with CKDIV8
#ifndef F_CPU
#define F_CPU 1000000UL     /* 1 Mhz internal oscillator  */
#endif


#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define IRPIRX	PB4
#define ORELAY	PB3
#define OLED	PB2
#define ORPITX	PB1
#define ISWITCH	PB0


#ifdef USE_MOSFET
// use mosfet - active low
	#define RELAY_ON	PORTB &= ~(1<<ORELAY)
	#define RELAY_OFF	PORTB |= (1<<ORELAY)
#else
// use relay - active high
	#define RELAY_ON	PORTB |= (1<<ORELAY)
	#define RELAY_OFF	PORTB &= ~(1<<ORELAY)
#endif


//internal states
enum sm_states_e {
	POWER_OFF = 0,
	STARTUP,
	RUNNING,
	SHUTDOWN
};


// modes for the LED
enum led_mode_e {
	LED_OFF=0,
	LED_BLINKING,
	LED_ON
};


// states of the button
enum button_state_e{
	NO_PRESS = 0,
	SHORT_PRESS,
	LONG_PRESS
};

//direction of soft blinking LED
enum led_pwm_direction_e{
	COUNT_UP = 0,
	COUNT_DOWN
};

// counter for button's debouncing and for detecting the long press
volatile uint16_t button_cnt = 0;

// counter for detecting the delay timer overflows
volatile uint8_t delay_counter = 0;

// actual mode of the LED
volatile enum led_mode_e led_mode = LED_OFF;


//level value for soft emulated PWM
uint8_t softblink_level=0;

//direction of soft blinking LED
enum led_pwm_direction_e led_pwm_dir = COUNT_UP;

//step for softblinking LED
#define SOFTBLINK_STEP 2


/**
 * Interrupt service routine for Timer 0.
 *
 * This timer is used to detect a long press of the button
 */
ISR( TIMER0_OVF_vect )                   // every 16ms
{
	button_cnt++;


	if(led_pwm_dir == COUNT_UP){
		if(softblink_level> (255-SOFTBLINK_STEP) ){
			led_pwm_dir = COUNT_DOWN;
			softblink_level = 255;
		} else {
			softblink_level += SOFTBLINK_STEP;
		}
	} else {
		if(softblink_level<SOFTBLINK_STEP){
			led_pwm_dir = COUNT_UP;
			softblink_level = 0;
		} else {
			softblink_level -= SOFTBLINK_STEP;
		}
	}
}

/**
 * Interrupt service routine for Timer 1.
 *
 * This timer is used to generate delay functionality.
 */
ISR(TIMER1_OVF_vect) {				//every 500ms
	//if(led_mode == LED_BLINKING)
	//	PORTB ^= (1<<OLED);
	delay_counter++;
}


ISR(INT0_vect) {

}

ISR(PCINT0_vect) {

}



int main(void) {

	//internal actual state of the state machine
	enum sm_states_e states_e = POWER_OFF;

	//actual state of the button
	enum button_state_e buttonstate = NO_PRESS;

	//temporary state which is used for resetting the delay counter
	enum sm_states_e temp_states_e = POWER_OFF;


	//temporary value for soft emulated PWM
	uint8_t softblink_temp=0;



	//set direction of pins to output
	DDRB = (1 << ORELAY) | (1 << OLED) | (1 << ORPITX);

	RELAY_OFF;		//Switch off relay

	PORTB &= ~(1<<ORPITX);		//Switch off TX


	// Configure debouncing routines
	DDRB  &= ~(1<<ISWITCH);                // configure key port for input
	PORTB |= (1<<ISWITCH);                // and turn on pull up resistors

	TCCR0B = (1<<CS01)|(1<<CS00);         // divide by 64
	// overflow 0.016 sec

	TIMSK |= 1<<TOIE0;                   // enable timer interrupt

	TCCR1 |= (1<<CS13) |(1<<CS12); // Prescaler 2048
	// overflow time about 0.5 sec

	PCMSK |= (1<<PCINT0); //  tell pin change mask to listen to PB0, Switch
	PCMSK |= (1<<PCINT4); //  tell pin change mask to listen to PB4, RPIInput

	// Overflow Interrupt erlauben
	TIMSK |= (1<<TOIE1);
	GIMSK |= (1<<PCIE);
	sei();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);


	//Detecting reset of microcontroller - for debugging
	PORTB &= ~(1<<OLED);
	for(int i=0; i < 10 ; i++){
		PORTB ^= 1<<OLED;
		_delay_ms(100);
	}
	PORTB |= (1<<OLED);


	while (1) {



		buttonstate=NO_PRESS;

		if( !(PINB & (1<<ISWITCH )) ) {
			button_cnt=0;
			while(button_cnt < 6){	//wait for about 50ms - debouncing

			}
			while(!(PINB & (1<<ISWITCH ))){
				buttonstate=SHORT_PRESS;
				if(button_cnt >= 250){	//4sec long button press-shutdown
					buttonstate=LONG_PRESS;
					break;
				}
			}
		}


		temp_states_e = states_e;


		//state transition function
		switch(states_e) {
		case POWER_OFF:
			if ( SHORT_PRESS == buttonstate )
				states_e = STARTUP;
			break;
		case STARTUP:
			if( (PINB & (1<<IRPIRX)) && delay_counter >= 6 ) {
				states_e = RUNNING;
			}
			if( delay_counter >= 120 ) {
				states_e = RUNNING;
			}
			break;
		case RUNNING:
			if( !(PINB &(1<<IRPIRX))) {
				states_e = SHUTDOWN;
			}
			if( SHORT_PRESS == buttonstate ) {
				states_e = SHUTDOWN;
			}
			break;
		case SHUTDOWN:
			if( (PINB &(1<<IRPIRX)) && delay_counter >= 4 ) {
				states_e = RUNNING;
			}
			if((!(PINB &(1<<IRPIRX))) && delay_counter >= 40 ) {	//20sec
				states_e = POWER_OFF;
			}
			if( delay_counter >= 120 ) {
				states_e = POWER_OFF;
			}
			break;
		}


		if ( LONG_PRESS == buttonstate ) {
			states_e = POWER_OFF;
		}


		if( temp_states_e != states_e) {
			delay_counter = 0;
		}

		//output function
		switch(states_e) {
		case POWER_OFF:
			RELAY_OFF;
			PORTB &= ~(1<<ORPITX);		//Switch off TX
			led_mode = LED_OFF;
			break;
		case STARTUP:
			RELAY_ON;
			PORTB &= ~(1<<ORPITX);		//Switch off TX
			led_mode = LED_BLINKING;
			break;
		case RUNNING:
			RELAY_ON;
			PORTB &= ~(1<<ORPITX);		//Switch off TX
			led_mode = LED_ON;
			break;
		case SHUTDOWN:
			RELAY_ON ;
			PORTB |= (1<<ORPITX);		//Switch on TX
			led_mode = LED_BLINKING;
			break;
		}


		//LED control functionality
		if(led_mode == LED_ON) {
			PORTB &= ~(1<<OLED);

			led_pwm_dir=COUNT_DOWN;
			softblink_level=255;

		} else if (led_mode == LED_OFF) {
			PORTB |= (1<<OLED);

			led_pwm_dir=COUNT_UP;
			softblink_level=0;

		} else if (led_mode == LED_BLINKING){
			//use a software emulated PWM to generate a soft blinking LED for startup
			if(softblink_temp < softblink_level) {
				PORTB |= (1<<OLED);
			} else {
				PORTB &= ~(1<<OLED);
			}
			softblink_temp++;
		}


		while(LONG_PRESS == buttonstate && (!(PINB & (1<<ISWITCH )))){	//if long button press
																//wait for release
		}



		cli();
		if (states_e == POWER_OFF || states_e == RUNNING) {
			sleep_enable();
			sei();
			sleep_cpu();
			sleep_disable();

			//sleep wakeup
			delay_counter = 0;
		}
		sei();


	}

	return 0;
}
