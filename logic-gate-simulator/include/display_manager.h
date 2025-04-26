#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "ssd1306.h"
#include <stdbool.h>

// Function prototypes
bool initialize_display(void);
void display_startup_screen(void);
void display_menu(int current_selection);
void display_gate_info(int gate_type);
void update_gate_simulation_display(int gate_type, bool input_a, bool input_b, bool output);

extern ssd1306_t disp;

#endif // DISPLAY_MANAGER_H