#include "hardware_config.h"

void initialize_peripherals() {
    // Initialize I2C for OLED display
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);

    // Initialize ADC for joystick
    adc_init();
    adc_gpio_init(ADC_Y);

    // Initialize buttons with pull-ups (active low)
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    // Initialize joystick button
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);

    // Initialize RGB LED pins
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
    
    // Turn off all LEDs initially
    set_led_color(false, false, false);
}

void set_led_color(bool red, bool green, bool blue) {
    gpio_put(LED_R, red);
    gpio_put(LED_G, green);
    gpio_put(LED_B, blue);
}