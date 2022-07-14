// Siehe auch: https://github.com/Sylaina/oled-display

#include "lcd.h"

uint8_t init_sequence[] = {
    0xAE,       // Display OFF (sleep mode)
    0x20, 0b00, // Set Memory Addressing Mode
    // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
    // 10=Page Addressing Mode (RESET); 11=Invalid
    0xB0,                     // Set Page Start Address for Page Addressing Mode, 0-7
    0xC8,                     // Set COM Output Scan Direction
    0x00,                     // --set low column address
    0x10,                     // --set high column address
    0x40,                     // --set start line address
    0x81, 0x3F,               // Set contrast control register
    0xA1,                     // Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
    0xA6,                     // Set display mode. A6=Normal; A7=Inverse
    0xA8, DISPLAY_HEIGHT - 1, // Set multiplex ratio(1 to 64)
    0xA4,                     // Output RAM to Display
                              // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
    0xD3, 0x00,               // Set display offset. 00 = no offset
    0xD5,                     // --set display clock divide ratio/oscillator frequency
    0xF0,                     // --set divide ratio
    0xD9, 0x22,               // Set pre-charge period
                              // Set com pins hardware configuration
    0xDA, 0x12,
    0xDB,       // --set vcomh
    0x20,       // 0x20,0.77xVcc
    0x8D, 0x14, // Set DC-DC enable
    0xAF};

void lcd_drawPixel(uint8_t x, uint8_t y)
{
    displayBuffer[(y / 8)][x] |= (1 << (y % 8));
}

void lcd_delPixel(uint8_t x, uint8_t y)
{
    displayBuffer[(y / 8)][x] &= ~(1 << (y % 8));
}

void lcd_command(uint8_t cmd[], uint8_t size)
{
    i2c_start(0x3C);
    for (uint8_t i = 0; i < size; i++)
    {
        i2c_byte(0x80); // 0x80 for command, 0x40 for data
        i2c_byte(cmd[i]);
    }
    i2c_stop();
}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
    uint8_t commandSequence[] = {0xb0 + y, 0x21, 0x00 + ((2 + x) & (0x0f)), 0x10 + (((2 + x) & (0xf0)) >> 4), 0x7f};
    lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_data(uint8_t data[], uint16_t size)
{
    i2c_start(0x3C);
    i2c_byte(0x40); // 0x80 for command, 0x40 for data
    for (uint16_t i = 0; i < size; i++)
    {
        i2c_byte(data[i]);
    }
    i2c_stop();
}

void lcd_display()
{
    for (uint8_t i = 0; i < DISPLAY_HEIGHT / 8; i++)
    {
        lcd_gotoxy(0, i);
        lcd_data(displayBuffer[i], sizeof(displayBuffer[i]));
    }
}

void lcd_clrscr()
{
    for (uint8_t i = 0; i < DISPLAY_HEIGHT / 8; i++)
    {
        memset(displayBuffer[i], 0x00, sizeof(displayBuffer[i]));
        lcd_gotoxy(0, i);
        lcd_data(displayBuffer[i], sizeof(displayBuffer[i]));
    }
    lcd_gotoxy(0, 0);
}

void lcd_init()
{
    lcd_command(init_sequence, sizeof(init_sequence));
    lcd_clrscr();
}