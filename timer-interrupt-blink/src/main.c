#include <stdio.h>
#include <stdbool.h>
#include <metal/cpu.h>
#include <metal/gpio.h>

#define RTC_FREQ 32768
#define RTC_SLEEP_TICKS RTC_FREQ * 1 // 1 Second
#define LED_PIN 5

struct metal_cpu *cpu;
struct metal_gpio *gpio_led;
bool led_state = true;


void timer_handler(int id, void *data)
{
    led_state = !led_state;
    metal_gpio_set_pin(gpio_led, LED_PIN, led_state);

    metal_cpu_set_mtimecmp(cpu, metal_cpu_get_mtime(cpu) + RTC_SLEEP_TICKS);
}

int main(void)
{
    struct metal_interrupt *cpu_intr;
    struct metal_interrupt *tmr_intr;
    int tmr_id;

    // Get gpio device handle, IC's GPIO 5 equals pin silkscreen 13 (see Schematic for red-v dev board)
    gpio_led = metal_gpio_get_device(0);

    // Disabling input is mandatory for using them as output
    metal_gpio_disable_input(gpio_led, LED_PIN);

    // Set as output
    metal_gpio_enable_output(gpio_led, LED_PIN);

    // Pins have more than one function, make sure we disconnect anything connected...
    metal_gpio_disable_pinmux(gpio_led, LED_PIN);

    // Turn on pin (LED Off - inverted!)
    metal_gpio_set_pin(gpio_led, LED_PIN, 1);

    // Get CPU Hart reference
    cpu = metal_cpu_get(metal_cpu_get_current_hartid());

    // Get Interrupt controllers
    cpu_intr = metal_cpu_interrupt_controller(cpu);
    tmr_intr = metal_cpu_timer_interrupt_controller(cpu);

    // Initialize Interrupts
    metal_interrupt_init(cpu_intr);
    metal_interrupt_init(tmr_intr);

    // Register Timer Interrupt handler
    tmr_id = metal_cpu_timer_get_interrupt_id(cpu);
    metal_interrupt_register_handler(tmr_intr, tmr_id, timer_handler, cpu);

    // Set Timeout
    metal_cpu_set_mtimecmp(cpu, metal_cpu_get_mtime(cpu) + RTC_SLEEP_TICKS);
    
    // Enable interrupts
    metal_interrupt_enable(tmr_intr, tmr_id);
    metal_interrupt_enable(cpu_intr, 0);

    return 0;
}