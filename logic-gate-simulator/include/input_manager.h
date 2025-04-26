#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdbool.h>
#include "pico/stdlib.h"

// Function prototypes
void setup_button_interrupts(void);
bool is_button_pressed(void);
void reset_button_pressed(void);
int read_joystick_position(void);
bool read_button_state(int button_pin);
uint32_t get_last_button_press_time(void);
void update_last_button_press_time(void);

#endif // INPUT_MANAGER_H