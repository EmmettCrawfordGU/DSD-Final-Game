// Header file for the hardware drivers
// Authors: Matthew VonWahlde, Cameron Zheng, Hayato Tsujii, Emmett Crawford
// Last modified: 12/7/2023
// Description : This file contains the function prototypes for the hardware drivers

#ifndef HW_H__
#define HW_H__


#include "system.h"
#include <sys/alt_stdio.h>
#include <sys/alt_irq.h>
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include <stdint.h>
#include <stdlib.h>


#define NUM_LCD_SETUP_COMMANDS 14 /* The number of commands needed to initialize the LCD */
#define LCD_TIMER               0 /* An ID for the LCD timer */
#define GAME_TIMER              1 /* An ID for the game timer */
#define CLOCK_FREQ       50000000 /* The frequency of the clock (50 MHz) */


// Function prototypes for hw.c
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
