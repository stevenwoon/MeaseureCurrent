#ifndef COMMON_H_
#define COMMON_H_

#define LED 3
#define BTN 0

#define SET(x) |= (1<<x)
#define CLR(x) &=~(1<<x)
#define INV(x) ^=(1<<x)

enum BOOL {
	TRUE,
	FALSE
};

enum TIMERS {
	tLED = 0,
	tDEBOUNCE = 1,
	tLONGPRESS = 2,
	NUM_TIMERS = 3
};

#endif