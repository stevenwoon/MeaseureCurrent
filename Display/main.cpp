#include "tm1637.h"
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "atod.h"
#define FILTER 1

int adc(void)
{
	uint8_t adc_lobyte; // to hold the low byte of the ADC register (ADCL)
	uint16_t raw_adc;

	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete

	// for 10-bit resolution:
	adc_lobyte = ADCL; // get the sample value from ADCL
	raw_adc = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte

	return raw_adc;
}



int main() {
	TM1637 disp;
	disp.setBrightness(0x0f);

	int i;
	initADC();
	volatile int value;
	volatile int value2;

	// 120 increment = 1000 mA
	// 1 step = 1000/120 mA
	float ma_step = 1000/120;
	while (1) {
/*
		value = 0;
		for(i = 0; i < FILTER; ++i) {
			value += (adc() - 512);
			_delay_ms(20);
		}
	  
		value = value/FILTER;
		*/
		value = adc();
		value2 = adc();
		if (((value - value2) < 1) && ((value2 - value) < 1)) {
			value += value2;
			value = (value / 2) - 526;
			if (value < 0) value = -value;
			value = (int) ((float) value *  ma_step);
			disp.clear();
			disp.setNumber(value);
			_delay_ms(200);
		}
	}
}
