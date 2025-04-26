#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// I2C configuration
#define I2C_PORT i2c1
#define PINO_SCL 15
#define PINO_SDA 14

// Input pins
#define BTN_A 5      // Button A pin (active low)
#define BTN_B 6      // Button B pin (active low)
#define SW 22        // Joystick button
#define ADC_Y 26     // Joystick Y-axis

// RGB LED pins
#define LED_R 13
#define LED_G 11
#define LED_B 12

#define DEBOUNCE_TIME 200

// Function prototypes
void initialize_peripherals(void);
void set_led_color(bool red, bool green, bool blue);

#endif // HARDWARE_CONFIG_H