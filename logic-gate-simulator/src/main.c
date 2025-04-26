#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware_config.h"
#include "display_manager.h"
#include "input_manager.h"
#include "menu_system.h"

int main() {
    stdio_init_all();
    initialize_peripherals();
    setup_button_interrupts();

    // Initialize OLED display
    if (!initialize_display()) {
        printf("Error: Failed to initialize display!\n");
        while (1);
    } else {
        printf("Display initialized successfully!\n");
    }

    // Startup message
    display_startup_screen();
    sleep_ms(1500);
    
    while (1) {
        run_menu_system();
        sleep_ms(100);  // Small delay for joystick debouncing
    }
}