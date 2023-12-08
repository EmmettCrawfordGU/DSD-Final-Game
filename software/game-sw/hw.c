// C code file for the hardware drivers
// Authors: Matthew VonWahlde, Cameron Zheng, Hayato Tsujii, Emmett Crawford
// Last modified: 12/7/2023
// Description : This file contains the hardware drivers.

#include "hw.h"


// LCD commands to initialize, in order
// Bits: 11 - LCD_EN, 10 - LCD_RW, 9 - LCD_RS, 8 - LCD_ON, 7-0 - data
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
	0x0106 // 0,0,0,1,00000110
};

// Array to display digits on the seven segment display
const uint32_t HexArray[] = {
	0xFFFFFFC0, // 0
	0xFFFFFFF9, // 1
	0xFFFFFFA4, // 2
	0xFFFFFFB0, // 3
	0xFFFFFF99, // 4
	0xFFFFFF92, // 5
	0xFFFFFF82, // 6
	0xFFFFFFF8, // 7
	0xFFFFFF80, // 8
	0xFFFFFF98  // 9
};

// Semaphores for delaying (set by their corresponding timers
uint8_t lcdSema = 1;
uint8_t gameSema = 1;


// Clear the semaphore inputted
void clearSema(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		lcdSema = 0;
	else if(timerNum == GAME_TIMER)
		gameSema = 0;
}


// Get the value of the semaphore inputted
uint8_t getSema(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		return lcdSema;
	else if(timerNum == GAME_TIMER)
		return gameSema;
	else
		return 0;
}


// ISR for the game timer
void handleGameTimer(void* isr_context){
	// Set the game semaphore
	gameSema = 1;

	// Acknowledge the interrupt
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_GAME_BASE, 0);
}


// ISR for the LCD timer
void handleLCDTimer(void* context){
	// Set the LCD semaphore
	lcdSema = 1;

	// Acknowledge the interrupt
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_LCD_BASE, 0);
}


// Return the value of switch 17
uint8_t readSwitch(void){
	return (uint8_t)IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
}


// Return the value of key 2
uint8_t readKey(void){
	return (uint8_t)IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE);
}


// Set the state of the LEDs
void setLEDs(uint8_t led_pattern){
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, (led_pattern&0x0F));
}


// Set the state of the four seven segment displays
void setSevenSeg(uint32_t num){
	int ones, tens, hundreds, thousands;

	// Calculate the values of the ones, tens, hundreds, and thousands place
	ones = num % 10;
	tens = (num/10) % 10;
	hundreds = (num/100) % 10;
	thousands = (num/1000) % 10;

	// Set value for the GPIO register
	uint32_t hex_data = 0xFFFFFFFF;
	if(num > 999)
		hex_data &= HexArray[thousands];
	if(num > 99)
		hex_data = (((hex_data << 7) | 0x7F) & HexArray[hundreds]);
	if(num > 9)
		hex_data = (((hex_data << 7) | 0x7F) & HexArray[tens]);

	hex_data = (((hex_data << 7) | 0x7F) & HexArray[ones]);

	// Display on the seven segment display
	IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASE, hex_data);
}


// Initializes the LCD
void lcdInit(void){
	// Set the LCD timer to 5 ms and start
	setTimer(LCD_TIMER,5000);
	startTimer(LCD_TIMER);

	// Write each initialization command to the LCD
	for(int i = 0; i < NUM_LCD_SETUP_COMMANDS; i++){
		while(lcdSema == 0){};
		lcdSema = 0;
		IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, (commands[i]&0x0FFF));
	}

	// Stop the timer once done
	stopTimer(LCD_TIMER);
}


// Sets the location of the LCD to write
void lcdSetAddr(uint8_t row, uint8_t col){
	// Create the command to send to the LCD
	// 1, 0, 1, 1, data
	uint16_t lcdData = 0x0000;
	lcdData = row * 0x40 + col;
	lcdData |= 0x80;
	lcdData |= 0x9 << 8;

	// Set the timer to 1 ms and start
	setTimer(LCD_TIMER, 1000);
	startTimer(LCD_TIMER);

	// Wait for 1 ms (lcdSema set to 1 by the LCD timer
	lcdSema = 0;
	while(lcdSema == 0){};

	// Write the data to the LCD
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	// Clear the enable bit to create a falling edge
	lcdData &= ~0x0800;

	// Wait for 1 ms
	lcdSema = 0;
	while(lcdSema == 0){};

	// Write the data to the LCD
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	// Stop the timer once done
	stopTimer(LCD_TIMER);
}


// Writes a null-terminated string to the LCD
void lcdWriteStr(char str[]){
	int i = 0;

	// Write each character to the LCD
	while(str[i] != '\0'){
		lcdWrite((uint8_t)str[i++]);
	}
}


// Writes a character to the LCD
void lcdWrite(uint8_t data){
	// Create the command to write a character to the LCD
	// 1, 0, 1, 1, data
	uint16_t lcdData = 0x0000;
	lcdData |= data;
	lcdData |= 0xB << 8;

	// Set the timer to 1 ms and start the timer
	setTimer(LCD_TIMER, 1000);
	startTimer(LCD_TIMER);

	// Wait for 1 ms (set by the lcd timer
	lcdSema = 0;
	while(lcdSema == 0){};

	// Write the data to the LCD
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	// Clear the enable bit
	lcdData &= ~0x0800;

	// Wait for 1 ms (set by the LCD timer)
	lcdSema = 0;
	while(lcdSema == 0){};

	// Waite the data to the LCD
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, lcdData);

	// Stop the timer when done
	stopTimer(LCD_TIMER);
}


// Clears the lcd
void lcdClear(){
	// Set the timer to 1.52 ms
	setTimer(LCD_TIMER, 1520);
	startTimer(LCD_TIMER);

	// Wait for 1.52 ms (set by the LCD semaphore)
	lcdSema = 0;
	while(lcdSema == 0){};

	// Send the command to clear (EN high)
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, 0x801);

	// Wait for 1.52 ms (set by the LCD semaphore)
	lcdSema = 0;
	while(lcdSema == 0){};

	// Send the command to clear (EN low)
	IOWR_ALTERA_AVALON_PIO_DATA(LCD_BASE, 0x001);

	// Stop the timer when done
	stopTimer(LCD_TIMER);
}


// Set the ISR's for the timers
void initTimers(void){
	alt_ic_isr_register(TIMER_GAME_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_GAME_IRQ, &handleGameTimer, NULL, NULL);
	alt_ic_isr_register(TIMER_LCD_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_LCD_IRQ, &handleLCDTimer, NULL, NULL);
}


// Starts the corresponding timer
void startTimer(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x0007);
	else if(timerNum == GAME_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_GAME_BASE, 0x0007);
}


// Stops the corresponding timer
void stopTimer(uint8_t timerNum){
	if(timerNum == LCD_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x000B);
	else if(timerNum == GAME_TIMER)
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_GAME_BASE, 0x000B);

}


// Sets the corresponding timer to a microsecond delay
void setTimer(uint8_t timerNum, uint32_t microsec){
	// Calculate the number of cycles to delay
	uint32_t cycles = millisToClock(microsec);

	// Stores the timer delay to the upper and lower registers
	uint16_t lower = (uint16_t)(cycles&0x0000FFFF);
	uint16_t upper = (uint16_t)((cycles>>16)&0x0000FFFF);

	// Set the registers for the corresponding timer
	if(timerNum == LCD_TIMER){
		IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LCD_BASE, lower);
		IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LCD_BASE, upper);
	}else if(timerNum == GAME_TIMER){
		IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_GAME_BASE, lower);
		IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_GAME_BASE, upper);
	}
}


// Converts from milliseconds to clock cycles
uint32_t millisToClock(uint32_t microsec){
	return CLOCK_FREQ / 1000000 * microsec;
}
