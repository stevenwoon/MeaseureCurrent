#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "pbut.h"
#include "global.h"

#define BTN_STATUS !((PINB >> BTN) & 0x01)
#define DEBOUNCE_TIME	50		// milliseconds
#define LONGPRESS_TIME	5000

extern uint16_t timers[];
BTN_STATE btn_state = btnOFF;
BTN_LONGPRESS_STATE btn_longpress_state = longpressOFF;
void pb_init()
{
	PORTB |= (1 << BTN);  // Enable PULL_UP resistor
	GIMSK |= (1 << PCIE); // Enable Pin Change Interrupts
	PCMSK |= (1 << BTN);  // Use PCINTn as interrupt pin
	sei();                // Enable interrupts
	DDRB |= (1 << DDB0); //Set pin 0 to output for the LED
}


ISR (PCINT0_vect)
{
    if (!((PINB >> BTN) & 0x01)) {
		timers[tDEBOUNCE] = DEBOUNCE_TIME;
		timers[tLONGPRESS] = LONGPRESS_TIME;
		btn_state = DEBOUNCING;
		btn_longpress_state = COUNTDOWN;
	}
	else {
		btn_state  = btnOFF;
		btn_longpress_state = longpressOFF;
	}
};