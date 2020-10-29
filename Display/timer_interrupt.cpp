#include <avr/interrupt.h>
#include <avr/io.h>
#include "global.h"

extern uint16_t timers[];



void setupTimer(void) {
	for (int i = 0; i < NUM_TIMERS; ++i) {
		timers[NUM_TIMERS] = 0;
	}
	
	DDRB SET(LED);
	PORTB CLR(LED);
	cli();

	// CTC mode. set WGM01
	TCCR0A CLR(WGM00);
	TCCR0A SET(WGM01);
	TCCR0B CLR(WGM02);

	// divide by 64
	TCCR0B SET(CS00);
	TCCR0B SET(CS01);
	TCCR0B CLR(CS02);

	// top value in ctc mode...
	OCR0A = 125; // 8,000,000 / 64 / 125 = 1ms

	TIMSK SET(OCIE0A); // set timer to interrupt on match...
//	TIMSK SET(TOIE0); // set timer to interrupt on overflow...

	sei();
}

// Interrupts every ms
ISR (TIMER0_COMPA_vect) {
	volatile uint8_t i;
	for(i = 0; i < NUM_TIMERS; ++i) {
		if (timers[i] > 0) {
			--timers[i];
		}
	}
}