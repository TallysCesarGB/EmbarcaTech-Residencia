#include "menu_system.h"
#include "display_manager.h"
#include "input_manager.h"
#include "logic_gates.h"
#include "hardware_config.h"
#include "pico/stdlib.h"

void run_menu_system(void) {
    static int current_selection = 0;
    
    // Display menu
    display_menu(current_selection);
    
    // Handle navigation
    current_selection = handle_menu_navigation(current_selection);
    
    // Check if user selected an option
    if (is_button_pressed()) {
        reset_button_pressed();
        simulate_logic_gate(current_selection);
    }
}

int handle_menu_navigation(int current_selection) {
    uint16_t y_val = read_joystick_position();
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    int new_selection = current_selection;
    
    if ((current_time - get_last_button_press_time()) > DEBOUNCE_TIME) {
        if (y_val < 1000) { // joystick para baixo
            new_selection = (current_selection + 1) % get_gate_count();
            update_last_button_press_time();
        } else if (y_val > 3000) { // joystick para cima
            new_selection = (current_selection - 1 + get_gate_count()) % get_gate_count();
            update_last_button_press_time();
        }
    }
    
    return new_selection;
}

void simulate_logic_gate(int gate_type) {
    bool exit_simulation = false;
    
    // Turn off LEDs initially
    set_led_color(false, false, false);
    
    // Display initial gate info
    display_gate_info(gate_type);
    
    // Wait a moment for user to read instructions
    sleep_ms(1500);
    
    while (!exit_simulation) {
        // Check if joystick button is pressed to exit
        if (is_button_pressed()) {
            reset_button_pressed();
            exit_simulation = true;
            continue;
        }
        
        // Read button states (remember they are active low - pressed = 0)
        bool input_a = read_button_state(BTN_A);
        bool input_b = read_button_state(BTN_B);
        
        // Compute gate output
        bool output = compute_logic_output(gate_type, input_a, input_b);
        
        // Update display with current state
        update_gate_simulation_display(gate_type, input_a, input_b, output);
        
        // Set LED color based on output (RED for 0, GREEN for 1)
        set_led_color(!output, output, false);
        
        // Small delay to debounce and prevent flickering
        sleep_ms(100);
    }
    
    // Turn off all LEDs when exiting
    set_led_color(false, false, false);
}