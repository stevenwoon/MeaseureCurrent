#include "tm1637.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/delay.h>

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/sleep.h>
#include "atod.h"
#include "pbut.h"
#include "global.h"
#include "timer_interrupt.h"

// CLK (PB2)	Pin 7
// DAT (PB1)	Pin 6
// ADC2 (PB4)	Pin 3
// PushButton (PB0) Pin 5

uint16_t timers[NUM_TIMERS];
BOOL btnPressed = FALSE;

void disableInterrupts() {
	TIMSK CLR(OCIE0A); // set timer to interrupt on match...
	GIMSK CLR(PCIE); // Enable Pin Change Interrupts
	PORTB CLR(BTN);  // Enable PULL_UP resistor

}

void enableInterrupts(){
	TIMSK SET(OCIE0A); // set timer to interrupt on match...
	GIMSK SET(PCIE); // Enable Pin Change Interrupts
	PORTB SET(BTN);  // Enable PULL_UP resistor
}

int adc(void)
{
	uint8_t adc_lobyte; // to hold the low byte of the ADC register (ADCL)
	uint16_t raw_adc;

	set_sleep_mode(SLEEP_MODE_ADC);
	sleep_enable();
	sleep_mode();
	
/*	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
*/
	// for 10-bit resolution:
	adc_lobyte = ADCL; // get the sample value from ADCL
	raw_adc = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte

	return raw_adc;
}

#define BAD_VALUE	9999
int getValue() {
	volatile int value;
	volatile int value2;

	disableInterrupts();
	value = adc();
	value2 = adc();
	enableInterrupts();
//	if (((value - value2) < 1) && ((value2 - value) < 1)) {
	if (abs(value - value2) < 1) {
		value += value2;
		return value;
	}
	return BAD_VALUE;
}

/* EEPROM addresses */
uint16_t ADC_OFFSET_ADDR = 0;
uint16_t MA_STEP_ADDR = 2;

int main() {
	TM1637 disp;
	int adc_offset = eeprom_read_word(&ADC_OFFSET_ADDR);
	int ma_step = eeprom_read_word(&MA_STEP_ADDR);
	disp.setBrightness(0x0f);

//	int i;
	initADC();
	pb_init();
	setupTimer();
	// 1 step = 1000/120 mA

	// 120 increment = 1000 mA
	volatile int value;
	//float ma_step = 1000/(eeprom_read_word();
	while (1) {	
		if (timers[tLED] == 0) {
			PORTB INV(LED);
			timers[0] = 1000;
		}
		
		// Check state of pushbutton
		if (btn_state == DEBOUNCING) {
			// Check if debouncing is completed
			if (timers[tDEBOUNCE] == 0) {
				btn_state = BTN_PRESSED;
				value = getValue();
				if (value != BAD_VALUE) {
					eeprom_update_word(&ADC_OFFSET_ADDR, value);
					adc_offset = value;
				}
				disp.clear();
				_delay_ms(1000);
			}
		}
		
		// Check if longpressed
		if (btn_longpress_state == COUNTDOWN) {
			if (timers[tLONGPRESS] == 0) {
				btn_longpress_state = BTN_LONGPRESSED;
				do {
					value = getValue();
				} while (value == BAD_VALUE);

				ma_step = abs(abs(value) - abs(adc_offset)) / 2;
				eeprom_update_word(&MA_STEP_ADDR, ma_step);
				disp.setNumber(ma_step);		
			}
		}

		if (ma_step == 0) {
			disp.setChars("Err0");
		} else {
			value = getValue();
			if (value != BAD_VALUE) {
				value = (value - adc_offset) / 2;	
				if (value < 0) value = -value;
				value = (int) ((float) value *  (1000 / ma_step));
				disp.clear();
				disp.setNumber(value);
				_delay_ms(200);
			}			
		}
	}
}

ISR(ADC_vect) {
	volatile int x = 0;
	++x;
}