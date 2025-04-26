#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "ssd1306.h"

#define I2C_PORT i2c1
#define PINO_SCL 15
#define PINO_SDA 14
#define BTN_A 5  // Button A pin (active low)
#define BTN_B 6  // Button B pin (active low)
#define SW 22     // Joystick button
#define ADC_Y 26  // Joystick Y-axis
#define DEBOUNCE_TIME 200

// RGB LED pins
#define LED_R 13
#define LED_G 11
#define LED_B 12

// Logic gate types
#define AND_GATE 0
#define OR_GATE 1
#define NOT_GATE 2
#define NAND_GATE 3
#define NOR_GATE 4
#define XOR_GATE 5
#define XNOR_GATE 6

const char *gate_names[] = {"AND", "OR", "NOT", "NAND", "NOR", "XOR", "XNOR"};
volatile bool button_pressed = false;
int current_selection = 0;
bool menu_active = true;
ssd1306_t disp;
uint32_t last_button_press = 0;

void button_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_button_press > DEBOUNCE_TIME) {
        button_pressed = true;
        last_button_press = current_time;
    }
}

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

    // Initialize joystick button with callback
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // Initialize RGB LED pins
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
    
    // Turn off all LEDs initially
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
}

void display_menu() {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 15, 5, 1, "Logic Gate Simulator");
    ssd1306_draw_line(&disp, 0, 15, 127, 15);
    
    // Definições para o menu com rolagem
    const int visible_items = 3;     // Número de itens visíveis por vez
    const int item_height = 15;      // Altura de cada item em pixels
    const int start_y = 25;          // Posição Y inicial dos itens
    
    // Calcular qual item deve ser o primeiro visível
    int first_visible = current_selection - 1;
    if (first_visible < 0) first_visible = 0;
    if (first_visible > 7 - visible_items) first_visible = 7 - visible_items;
    
    // Desenhar indicador de rolagem para cima (se necessário)
    if (first_visible > 0) {
        ssd1306_draw_string(&disp, 60, 17, 1, "^");
    }
    
    // Desenhar os itens visíveis do menu
    for (int i = 0; i < visible_items; i++) {
        int menu_index = first_visible + i;
        if (menu_index < 7) {  // Garantir que não ultrapasse o número de itens
            if (menu_index == current_selection) {
                ssd1306_draw_string(&disp, 10, start_y + i * item_height, 1, ">");
            }
            ssd1306_draw_string(&disp, 25, start_y + i * item_height, 1, gate_names[menu_index]);
        }
    }
    
    // Desenhar indicador de rolagem para baixo (se necessário)
    if (first_visible + visible_items < 7) {
        ssd1306_draw_string(&disp, 60, start_y + visible_items * item_height - 8, 1, "v");
    }
    
    ssd1306_show(&disp);
}
bool compute_logic_output(int gate_type, bool input_a, bool input_b) {
    switch(gate_type) {
        case AND_GATE:
            return input_a && input_b;
        case OR_GATE:
            return input_a || input_b;
        case NOT_GATE:
            return !input_a;
        case NAND_GATE:
            return !(input_a && input_b);
        case NOR_GATE:
            return !(input_a || input_b);
        case XOR_GATE:
            return input_a != input_b;
        case XNOR_GATE:
            return input_a == input_b;
        default:
            return false;
    }
}

void simulate_logic_gate() {
    bool exit_simulation = false;
    
    // Turn off LEDs initially
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
    
    // Display initial gate info
    ssd1306_clear(&disp);
    char buffer[50];
    sprintf(buffer, "%s Gate Simulation", gate_names[current_selection]);
    ssd1306_draw_string(&disp, 5, 5, 1, buffer);
    ssd1306_draw_line(&disp, 0, 15, 127, 15);
    
    if (current_selection == NOT_GATE) {
        ssd1306_draw_string(&disp, 5, 20, 1, "Use only Button A");
    } else {
        ssd1306_draw_string(&disp, 5, 20, 1, "Use Buttons A and B");
    }

    ssd1306_show(&disp);
    
    // Wait a moment for user to read instructions
    sleep_ms(1500);
    
    while (!exit_simulation) {
        // Check if joystick button is pressed to exit
        if (button_pressed) {
            button_pressed = false;
            exit_simulation = true;
            continue;
        }
        
        // Read button states (remember they are active low - pressed = 0)
        bool input_a = gpio_get(BTN_A);  // 0 when pressed, 1 when not pressed
        bool input_b = gpio_get(BTN_B);  // 0 when pressed, 1 when not pressed
        

        
        // Compute gate output
        bool output = compute_logic_output(current_selection, input_a, input_b);
        
        // Update display with current state
        ssd1306_clear(&disp);
        sprintf(buffer, "%s Gate", gate_names[current_selection]);
        ssd1306_draw_string(&disp, 5, 5, 1, buffer);
        ssd1306_draw_line(&disp, 0, 15, 127, 15);
        
        // Draw truth table header
        ssd1306_draw_string(&disp, 5, 20, 1, "INPUT");
        ssd1306_draw_string(&disp, 90, 20, 1, "OUTPUT");
        
        // Draw input states
        ssd1306_draw_string(&disp, 5, 30, 1, "A:");
        ssd1306_draw_string(&disp, 20, 30, 1, input_a ? "1" : "0");
        
        if (current_selection != NOT_GATE) {
            ssd1306_draw_string(&disp, 5, 40, 1, "B:");
            ssd1306_draw_string(&disp, 20, 40, 1, input_b ? "1" : "0");
        }
        
        // Draw output state
        ssd1306_draw_string(&disp, 100, 35, 1, output ? "1" : "0");
        
        ssd1306_show(&disp);
        
        // Set LED color based on output (RED for 0, GREEN for 1)
        gpio_put(LED_R, !output);  // Light red for 0
        gpio_put(LED_G, output);   // Light green for 1
        gpio_put(LED_B, 0);        // Blue off
        
        // Small delay to debounce and prevent flickering
        sleep_ms(100);
    }
    
    // Turn off all LEDs when exiting
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
}

int main() {
    stdio_init_all();
    initialize_peripherals();

    // Initialize OLED display
    if (!ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT)) {
        printf("Error: Failed to initialize display!\n");
        while (1);
    } else {
        printf("Display initialized successfully!\n");
    }

    // Startup message
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 10, 20, 1, "Logic Gate Simulator");
    ssd1306_draw_string(&disp, 40, 30, 1, "BitDogLab");
    ssd1306_draw_string(&disp, 25, 45, 1, "Starting...");
    ssd1306_show(&disp);
    sleep_ms(1500);
    
    while (1) {
        if (menu_active) {
            // Display menu and handle selection
            display_menu();
            
            // Read joystick for menu navigation
            adc_select_input(0);  // Y-axis
            uint16_t y_val = adc_read();
            
            // Move selection based on joystick position
            uint32_t current_time = to_ms_since_boot(get_absolute_time());
            if ((current_time - last_button_press) > DEBOUNCE_TIME) {
                if (y_val < 1000) { // joystick para baixo
                    current_selection = (current_selection + 1) % 7; // ciclo para a primeira opção se passar da última
                    last_button_press = current_time;
                } else if (y_val > 3000) { // joystick para cima
                    current_selection = (current_selection - 1 + 7) % 7; // ciclo para a última opção se estiver na primeira
                    last_button_press = current_time;
                }
            }
            
            // Check if joystick button is pressed to select a gate
            if (button_pressed) {
                button_pressed = false;
                menu_active = false;  // Exit menu mode, enter simulation mode
            }
            
            sleep_ms(100);  // Small delay for joystick debouncing
        } else {
            // Run the selected logic gate simulation
            simulate_logic_gate();
            menu_active = true;  // Return to menu after simulation
        }
    }
}