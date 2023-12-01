#include "hw.h"


int main(){
	uint8_t switch_data;
	initTimers();
	lcdInit();

	alt_putstr("Ciao from Nios II!\n");

	setTimer(GAME_TIMER, 10000);
	startTimer(GAME_TIMER);

	while(1) {
		clearSema(GAME_TIMER);
		while(getSema(GAME_TIMER) == 0){};
		switch_data = readSwitch();

		lcdClear();
		lcdSetAddr((~switch_data) & 0x01, 1);
		lcdWrite(0xF6);
	}

	return 0;
}
