#ifndef HW_H__
#define HW_H__


#include "system.h"
#include <sys/alt_stdio.h>
#include <sys/alt_irq.h>
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include <stdint.h>
#include <stdlib.h>


#define NUM_LCD_SETUP_COMMANDS 14
#define LCD_TIMER  0
#define GAME_TIMER 1
#define CLOCK_FREQ 50000000


void clearSema(uint8_t timerNum);
uint8_t getSema(uint8_t timerNum);
void handleGameTimer(void* isr_context);
void handleLCDTimer(void* context);
uint8_t readSwitch(void);
uint8_t readKey(void);
void setLEDs(uint8_t led_pattern);
void setSevenSeg(uint32_t num);
void lcdInit(void);
void lcdSetAddr(uint8_t row, uint8_t col);
void lcdWriteStr(char str[]);
void lcdWrite(uint8_t data);
void lcdClear();
void initTimers(void);
void startTimer(uint8_t timerNum);
void stopTimer(uint8_t timerNum);
void setTimer(uint8_t timerNum, uint32_t microsec);
uint32_t millisToClock(uint32_t microsec);


#endif
