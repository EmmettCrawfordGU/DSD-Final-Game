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


//typedef void(*handleGameTimer)(void* isr_context);

void handleGameTimer(void* isr_context){
	alt_putstr("Hello from Timer_Game\n");
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_GAME_BASE, 0);
}


void handleLCDTimer(void* context){
	alt_putstr("Hello from Timer_LCD\n");
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_LCD_BASE, 0);
}

void initTimers(void){
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_GAME_BASE, 0xF07F);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_GAME_BASE, 0x02FA);

	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_LCD_BASE, 0xE0FF);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_LCD_BASE, 0x05F5);

	alt_ic_isr_register(TIMER_GAME_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_GAME_IRQ, &handleGameTimer, NULL, NULL);
	alt_ic_isr_register(TIMER_LCD_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_LCD_IRQ, &handleLCDTimer, NULL, NULL);

	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_GAME_BASE, 0x0007);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_LCD_BASE, 0x0007);
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

int main(){
	uint8_t switch_data;
	uint8_t key_data;
	int delay;
	int led_pattern=0x0;
	char data[8];


	alt_putstr("Ciao from Nios II!\n");


	while(1) {
		switch_data = readSwitch();
		key_data = readKey();
		alt_putstr("Switch Data: ");
		alt_putstr(itoa(switch_data, data, 2));
		alt_putstr("\n");
		alt_putstr("Key Data: ");
		alt_putstr(itoa(key_data, data, 2));
		alt_putstr("\n");
		alt_putstr("\n");

		delay = 0;
		while(delay < 200000000) {
			delay++;
		}
		led_pattern ^= 0x0F; // toggle LEDs on selected switches
	}

	return 0;
}
