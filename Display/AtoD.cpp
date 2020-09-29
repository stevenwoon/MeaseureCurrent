void initADC()
{

  /* this function initialises the ADC 

  For more information, see table 17.5 "ADC Prescaler Selections" in 
  chapter 17.13.2 "ADCSRA – ADC Control and Status Register A"
  (pages 140 and 141 on the complete ATtiny25/45/85 datasheet, Rev. 2586M–AVR–07/10)

  
  // 10-bit resolution
  // set ADLAR to 0 to disable left-shifting the result (bits ADC9 + ADC8 are in ADC[H/L] and 
  // bits ADC7..ADC0 are in ADC[H/L])
  // use uint16_t variable to read ADC (intead of ADCH or ADCL) 
  
  */
#include <avr/io.h>

  ADMUX =
	    (0 << ADLAR) |     // do not left shift result (for 10-bit values)
            (0 << REFS2) |     // Sets ref. voltage to Vcc, bit 2
            (0 << REFS1) |     // Sets ref. voltage to Vcc, bit 1   
            (0 << REFS0) |     // Sets ref. voltage to Vcc, bit 0
            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

  ADCSRA = 
            (1 << ADEN)  |     // Enable ADC 
            (1 << ADPS2) |     // set prescaler to 16, bit 2 
            (0 << ADPS1) |     // set prescaler to 16, bit 1 
            (0 << ADPS0);      // set prescaler to 16, bit 0  
}
