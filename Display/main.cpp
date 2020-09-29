#include "tm1637.h"
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "atod.h"

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
  disp.setBrightness(0x08);

//  uint16_t i = 0;
  initADC();



 //  disp.clear();

  while (1) {
//	  disp.setNumber(i++);
	disp.setNumber(adc());
	  _delay_ms(1000);
//	  if (i > 9999) i = 0;
   }
//  disp.scrollChars("Hello World");

//  disp.setChars("done");
}
