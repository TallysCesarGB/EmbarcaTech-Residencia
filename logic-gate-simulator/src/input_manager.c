#include "input_manager.h"
#include "hardware_config.h"
#include "hardware/adc.h"

volatile bool button_pressed = false;
static uint32_t last_button_press = 0;

void button_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_button_press > DEBOUNCE_TIME) {
        button_pressed = true;
        last_button_press = current_time;
    }
}

void setup_button_interrupts(void) {
    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &button_isr);
}

bool is_button_pressed(void) {
    return button_pressed;
}

void reset_button_pressed(void) {
    button_pressed = false;
}

int read_joystick_position(void) {
    adc_select_input(0);  // Y-axis
    return adc_read();
}

bool read_button_state(int button_pin) {
    return gpio_get(button_pin);
}

uint32_t get_last_button_press_time(void) {
    return last_button_press;
}

void update_last_button_press_time(void) {
    last_button_press = to_ms_since_boot(get_absolute_time());
}