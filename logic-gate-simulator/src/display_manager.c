#include "display_manager.h"
#include "logic_gates.h"
#include <stdio.h>

#define I2C_PORT i2c1

ssd1306_t disp;

bool initialize_display(void) {
    return ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
}

void display_startup_screen(void) {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 6, 20, 1, "Logic Gate Simulator");
    ssd1306_draw_string(&disp, 40, 30, 1, "BitDogLab");
    ssd1306_draw_string(&disp, 25, 45, 1, "Starting...");
    ssd1306_show(&disp);
}

void display_menu(int current_selection) {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 4, 5, 1, "Logic Gate Simulator");
    ssd1306_draw_line(&disp, 0, 15, 127, 15);
    
    // Settings for the scrollable menu
    const int visible_items = 3;     // Number of items visible at a time
    const int item_height = 15;      // Height of each item in pixels
    const int start_y = 25;          // Initial Y position of items
    
    // Calculate which item should be visible first
    int first_visible = current_selection - 1;
    if (first_visible < 0) first_visible = 0;
    if (first_visible > get_gate_count() - visible_items) first_visible = get_gate_count() - visible_items;
    
    // Draw scroll up indicator (if needed)
    if (first_visible > 0) {
        ssd1306_draw_string(&disp, 60, 17, 1, "^");
    }
    
    // Draw visible menu items
    for (int i = 0; i < visible_items; i++) {
        int menu_index = first_visible + i;
        if (menu_index < get_gate_count()) {  // Ensure that you do not exceed the number of items
            if (menu_index == current_selection) {
                ssd1306_draw_string(&disp, 10, start_y + i * item_height, 1, ">");
            }
            ssd1306_draw_string(&disp, 25, start_y + i * item_height, 1, get_gate_name(menu_index));
        }
    }
    
    // Draw scroll down indicator (if needed)
    if (first_visible + visible_items < get_gate_count()) {
        ssd1306_draw_string(&disp, 60, start_y + visible_items * item_height - 8, 1, "v");
    }
    
    ssd1306_show(&disp);
}

void display_gate_info(int gate_type) {
    ssd1306_clear(&disp);
    char buffer[50];
    sprintf(buffer, "%s Gate Simulation", get_gate_name(gate_type));
    ssd1306_draw_string(&disp, 5, 5, 1, buffer);
    ssd1306_draw_line(&disp, 0, 15, 127, 15);
    
    if (gate_type == NOT_GATE) {
        ssd1306_draw_string(&disp, 5, 20, 1, "Use only Button A");
    } else {
        ssd1306_draw_string(&disp, 5, 20, 1, "Use Buttons A and B");
    }

    ssd1306_show(&disp);
}

void update_gate_simulation_display(int gate_type, bool input_a, bool input_b, bool output) {
    char buffer[50];
    ssd1306_clear(&disp);
    sprintf(buffer, "%s Gate", get_gate_name(gate_type));
    ssd1306_draw_string(&disp, 5, 5, 1, buffer);
    ssd1306_draw_line(&disp, 0, 15, 127, 15);
    
    // Draw truth table header
    ssd1306_draw_string(&disp, 5, 20, 1, "INPUT");
    ssd1306_draw_string(&disp, 90, 20, 1, "OUTPUT");
    
    // Draw input states
    ssd1306_draw_string(&disp, 5, 30, 1, "A:");
    ssd1306_draw_string(&disp, 20, 30, 1, input_a ? "1" : "0");
    
    if (gate_type != NOT_GATE) {
        ssd1306_draw_string(&disp, 5, 40, 1, "B:");
        ssd1306_draw_string(&disp, 20, 40, 1, input_b ? "1" : "0");
    }
    
    // Draw output state
    ssd1306_draw_string(&disp, 100, 35, 1, output ? "1" : "0");
    
    ssd1306_show(&disp);
}