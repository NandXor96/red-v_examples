#include <time.h>

#define DELAY 1000000 // 1 second
#define LED_PIN 5
#define GPIO_BASE_ADDRESS 0x10012000
#define GPIO_INPUT_EN GPIO_BASE_ADDRESS + 0x04
#define GPIO_OUTPUT_EN GPIO_BASE_ADDRESS + 0x08
#define GPIO_IOF_EN GPIO_BASE_ADDRESS + 0x38
#define GPIO_OUTPUT_VAL GPIO_BASE_ADDRESS + 0x0C

void delay(int number_of_microseconds)
{ // not actually number of seconds

    clock_t start_time = clock();

    // looooooooop to do nooooooooothing
    while (clock() < start_time + number_of_microseconds)
        ;
}

int main()
{
    unsigned int *gpio_input_en = (unsigned int *)(GPIO_INPUT_EN);
    unsigned int *gpio_output_en = (unsigned int *)(GPIO_OUTPUT_EN);
    unsigned int *gpio_disable_iof = (unsigned int *)(GPIO_IOF_EN);
    unsigned int *gpio_output_val = (unsigned int *)(GPIO_OUTPUT_VAL);

    *gpio_input_en &= 0 << LED_PIN;
    *gpio_output_en |= 1 << LED_PIN;
    *gpio_disable_iof &= ~(1 << LED_PIN);
    *gpio_output_val |= 1 << LED_PIN;

    while(1) {
        delay(DELAY);
        *gpio_output_val ^= 1 << LED_PIN;
    }
}