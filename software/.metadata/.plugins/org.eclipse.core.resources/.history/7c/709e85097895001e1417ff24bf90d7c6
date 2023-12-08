#include "hw.h"


const uint16_t commands[NUM_LCD_SETUP_COMMANDS] = {
	0x0838,// 1,0,0,0,00111000
	0x0038,// 0,0,0,0,00111000
	0x0838,// 1,0,0,0,00111000
	0x0038,// 0,0,0,0,00111000
	0x0838,// 1,0,0,0,00111000
	0x0038,// 0,0,0,0,00111000
	0x0838,// 1,0,0,0,00111000
	0x0038,// 0,0,0,0,00111000
	0x0801,// 1,0,0,0,00000001
	0x0001,// 0,0,0,0,00000001
	0x080C,// 1,0,0,0,00001100
	0x000C,// 0,0,0,0,00001100
	0x0906,// 1,0,0,1,00000110
	0x0106// 0,0,0,1,00000110
};


const uint32_t HexArray[] = {
	0xFFFFFFC0,
	0xFFFFFFF9,
	0xFFFFFFA4,
	0xFFFFFFB0,
	0xFFFFFF99,
	0xFFFFFF92,
	0xFFFFFF82,
	0xFFFFFFF8,
	0xFFFFFF80,
	0xFFFFFF98
};


uint8_t lcdSema = 1;
uint8_t gameSema = 1;


void clearSema(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		lcdSema = 0;
	else if(timerNum == GAME_TIMER)
		gameSema = 0;
}


uint8_t getSema(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		return lcdSema;
	else if(timerNum == GAME_TIMER)
		return gameSema;
	else
		return 0;
}


void handleGameTimer(void* isr_context){
	//alt_putstr("Hello from Timer_Game\n");
	gameSema = 1;

	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_GAME_BASE, 0);
}


void handleLCDTimer(void* context){
	lcdSema = 1;

	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_LCD_BASE, 0);
}


uint8_t readSwitch(void){
	return (uint8_t)IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
}

uint8_t readKey(void){
	return (uint8_t)IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE);
}


void setLEDs(uint8_t led_pattern){
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, (led_pattern&0x0F));
}


void setSevenSeg(uint32_t num){
	int ones, tens, hundreds, thousands;

	ones = num % 10;
	tens = (num/10) % 10;
	hundreds = (num/100) % 10;
	thousands = (num/1000) % 10;

	uint32_t hex_data = 0xFFFFFFFF;
	if(num > 999)
		hex_data &= HexArray[thousands];
	if(num > 99)
		hex_data = (((hex_data << 7) | 0x7F) & HexArray[hundreds]);
	if(num > 9)
		hex_data = (((hex_data << 7) | 0x7F) & HexArray[tens]);

	hex_data = (((hex_data << 7) | 0x7F) & HexArray[ones]);

	IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASE, hex_data);
}


// LCD_EN, LCD_RW, LCD_RS, LCD_ON, data
void lcdInit(void){
	setTimer(LCD_TIMER,5000);
	startTimer(LCD_TIMER);

	for(int i = 0; i < NUM_LCD_SETUP_COMMANDS; i++){
		while(lcdSema == 0){};
		lcdSema = 0;
		IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, (commands[i]&0x0FFF));
	}

	stopTimer(LCD_TIMER);
}


void lcdSetAddr(uint8_t row, uint8_t col){
	// 1, 0, 1, 1, data
	uint16_t lcdData = 0x0000;
	lcdData = row * 0x40 + col;
	lcdData |= 0x80;
	lcdData |= 0x9 << 8;

	lcdSema = 0;

	setTimer(LCD_TIMER, 1000);
	startTimer(LCD_TIMER);

	while(lcdSema == 0){};
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	lcdData &= ~0x0800;
	lcdSema = 0;
	while(lcdSema == 0){};
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	stopTimer(LCD_TIMER);
}


void lcdWriteStr(char str[]){
	int i = 0;
	while(str[i] != '\0'){
		lcdWrite((uint8_t)str[i++]);
	}
}


void lcdWrite(uint8_t data){
	// 1, 0, 1, 1, data
	uint16_t lcdData = 0x0000;
	lcdData |= data;
	lcdData |= 0xB << 8;

	lcdSema = 0;

	setTimer(LCD_TIMER, 1000);
	startTimer(LCD_TIMER);
	//alt_putstr(itoa(lcdData, cData, 2));
	//alt_putstr("\n");
	while(lcdSema == 0){};
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	lcdData &= ~0x0800;
	//alt_putstr(itoa(lcdData, cData, 2));
	//alt_putstr("\n");
	lcdSema = 0;
	while(lcdSema == 0){};
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	stopTimer(LCD_TIMER);
}


void lcdClear(){
	lcdSema = 0;

	setTimer(LCD_TIMER, 1520);
	startTimer(LCD_TIMER);

	while(lcdSema == 0){};
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, 0x801);

	lcdSema = 0;
	while(lcdSema == 0){};
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, 0x001);

	stopTimer(LCD_TIMER);
}


void initTimers(void){
	alt_ic_isr_register(TIMER_GAME_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_GAME_IRQ, &handleGameTimer, NULL, NULL);
	alt_ic_isr_register(TIMER_LCD_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_LCD_IRQ, &handleLCDTimer, NULL, NULL);
}


void startTimer(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x0007);
	else if(timerNum == GAME_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_GAME_BASE, 0x0007);
}


void stopTimer(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x000B);
	else if(timerNum == GAME_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_GAME_BASE, 0x000B);

}


void setTimer(uint8_t timerNum, uint32_t microsec){
	uint32_t cycles = millisToClock(microsec);

	uint16_t lower = (uint16_t)(cycles&0x0000FFFF);
	uint16_t upper = (uint16_t)((cycles>>16)&0x0000FFFF);

	if(timerNum == LCD_TIMER){
		IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LCD_BASE, lower);
		IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LCD_BASE, upper);
	}else if(timerNum == GAME_TIMER){
		IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_GAME_BASE, lower);
		IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_GAME_BASE, upper);
	}
}


uint32_t millisToClock(uint32_t microsec){
	return CLOCK_FREQ / 1000000 * microsec;
}
