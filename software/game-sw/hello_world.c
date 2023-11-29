#include "system.h"
#include <sys/alt_stdio.h>
#include <sys/alt_irq.h>
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include <stdint.h>
#include <stdlib.h>

#define SEVEN_SEG_0 0xFFFFF
#define SEVEN_SEG_1 0xFFFFFFF9 // 111...1111001
#define SEVEN_SEG_2 0xFFFFFFA4 // 111...0100100
#define SEVEN_SEG_3 0xFFFFFFB0 // 111...0110000
#define SEVEN_SEG_4 0xFFFFFF99 // 111...0011001
#define SEVEN_SEG_5 0xFFFFF
#define SEVEN_SEG_6 0xFFFFF
#define SEVEN_SEG_7 0xFFFFF
#define SEVEN_SEG_8 0xFFFFF
#define SEVEN_SEG_9 0xFFFFF

#define LCD_TIMER  0
#define GAME_TIMER 1
#define CLOCK_FREQ 50000000

uint32_t millisToClock(uint32_t microsec);
void startTimer(uint8_t timerNum);
void stopTimer(uint8_t timerNum);
void setTimer(uint8_t timerNum, uint32_t microsec);


uint32_t millisToClock(uint32_t microsec){
	return CLOCK_FREQ / 1000000 * microsec;
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

uint8_t lcdSema = 1;
uint8_t gameSema = 1;

//typedef void(*handleGameTimer)(void* isr_context);

void handleGameTimer(void* isr_context){
	//alt_putstr("Hello from Timer_Game\n");
	gameSema = 1;

	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_GAME_BASE, 0);
}


void handleLCDTimer(void* context){
	lcdSema = 1;

	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_LCD_BASE, 0);
}

void initTimers(void){
	alt_ic_isr_register(TIMER_GAME_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_GAME_IRQ, &handleGameTimer, NULL, NULL);
	alt_ic_isr_register(TIMER_LCD_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_LCD_IRQ, &handleLCDTimer, NULL, NULL);
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

void setSevenSeg(uint8_t thousands, uint8_t hundreds, uint8_t tens, uint8_t ones){
	uint32_t hex_data = 0xFFFFFFFF;
	hex_data &= SEVEN_SEG_1;
	hex_data = (((hex_data << 7) | 0x7F) & SEVEN_SEG_2);
	hex_data = (((hex_data << 7) | 0x7F) & SEVEN_SEG_3);
	hex_data = (((hex_data << 7) | 0x7F) & SEVEN_SEG_4);

	IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASE, hex_data);
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

void lcdSetAddr(uint8_t row, uint8_t col){
	// 1, 0, 1, 1, data
	uint16_t lcdData = 0x0000;
	lcdData = row * 0x40 + col;
	lcdData |= 0x80;
	lcdData |= 0x9 << 8;
	char cData[16];

	lcdSema = 0;

	setTimer(LCD_TIMER, 1000);
	startTimer(LCD_TIMER);
	//alt_putstr("\n");
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

void lcdWrite(uint8_t data){
	// 1, 0, 1, 1, data
	uint16_t lcdData = 0x0000;
	lcdData |= data;
	lcdData |= 0xB << 8;
	char cData[16];

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

// LCD_EN, LCD_RW, LCD_RS, LCD_ON, data
void lcdInit(void){

	#define NUM_LCD_SETUP_COMMANDS 14

	//IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LCD_BASE, 0xD08F);
	//IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LCD_BASE, 0x0003);
	setTimer(LCD_TIMER,5000);

	//IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x0007);
	startTimer(LCD_TIMER);

	uint16_t commands[NUM_LCD_SETUP_COMMANDS] = {
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

	for(int i = 0; i < NUM_LCD_SETUP_COMMANDS; i++){
		while(lcdSema == 0){};
		lcdSema = 0;
		IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, (commands[i]&0x0FFF));
	}


	//IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x000B);
	stopTimer(LCD_TIMER);
}

int main(){
	uint8_t switch_data;
	char data[8];
	initTimers();
	lcdInit();

	alt_putstr("Ciao from Nios II!\n");

	setTimer(GAME_TIMER, 10000);
	startTimer(GAME_TIMER);

	while(1) {
		gameSema = 0;
		while(gameSema == 0){};
		switch_data = readSwitch();

		lcdClear();
		lcdSetAddr((~switch_data) & 0x01, 1);
		//alt_putstr(itoa(switch_data, data, 2));
		//alt_putstr("\n");
		lcdWrite(0xF6);
	}

	return 0;
}
