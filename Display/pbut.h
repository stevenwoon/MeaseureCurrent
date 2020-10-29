#ifndef PBUT_H_
#define PBUT_H_

void pb_init();
enum BTN_STATE {
	btnOFF,
	DEBOUNCING,
	BTN_PRESSED
};

enum BTN_LONGPRESS_STATE {
	longpressOFF,
	COUNTDOWN,
	BTN_LONGPRESSED
};

extern BTN_STATE btn_state;
extern BTN_LONGPRESS_STATE btn_longpress_state;

#endif /* PBUT_H_ */