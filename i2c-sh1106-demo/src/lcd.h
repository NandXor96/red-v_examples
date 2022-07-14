// Siehe auch: https://github.com/Sylaina/oled-display

#include <stdio.h>
#include "i2c.h"
#include <string.h>

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 128

uint8_t init_sequence[];

static uint8_t displayBuffer[DISPLAY_HEIGHT / 8][DISPLAY_WIDTH];

void lcd_drawPixel(uint8_t x, uint8_t y);

void lcd_delPixel(uint8_t x, uint8_t y);

void lcd_command(uint8_t cmd[], uint8_t size);

void lcd_gotoxy(uint8_t x, uint8_t y);

void lcd_data(uint8_t data[], uint16_t size);

void lcd_display();

void lcd_clrscr();

void lcd_init();