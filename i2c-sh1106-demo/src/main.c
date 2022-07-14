// Siehe auch: https://github.com/Sylaina/oled-display

#include <stdio.h>
#include "i2c.h"
#include "lcd.h"

int main()
{
    i2c_init();

    lcd_init();

    uint32_t pixels[32] = {0x00000000, 0x007F81FE, 0x00400010, 0x00700010, 0x004001FE, 0x007F8000, 0x000001D0, 0x007C0150, 0x00540150, 0x005C01F0, 0x00000000, 0x007F81FE, 0x00400100, 0x00000000, 0x007F81FE, 0x00480100, 0x00400000, 0x000001F0, 0x006F8110, 0x006F8110, 0x000001F0, 0x006F8000, 0x006F8000, 0x00000000, 0x006F8000, 0x006F8000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

    for (int i = 0; i < 128; i += 32)
    {
        for (int j = 0; j < 64; j += 32)
        {
            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 32; x++)
                {
                    if (pixels[x] & (1 << y))
                    {
                        lcd_drawPixel(x + i, y + j);
                    }
                }
            }
        }
    }

    lcd_display();
}
