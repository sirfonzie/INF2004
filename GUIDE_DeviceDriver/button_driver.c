#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BUTTON_PIN 2
#define DEBOUNCE_TIME_MS 50

// Volatile variables used within the interrupt handler
static volatile uint32_t last_press_time = 0;
static volatile bool button_pressed_flag = false;

// Hardware Interrupt (IRQ) Handler for GPIO events
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // Debounce: Ignore extra triggers within the 50ms window
        if (current_time - last_press_time > DEBOUNCE_TIME_MS) {
            last_press_time = current_time;
            button_pressed_flag = true; // Signal main loop
        }
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Give USB serial time to connect

    printf("Initializing Button Driver...\n");

    // 1. Initialize the button pin
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    
    // 2. Enable Pull-up resistor (Button should be wired between Pin 2 and GND)
    gpio_pull_up(BUTTON_PIN);

    // 3. Configure interrupt on falling edge (when button is pressed to GND)
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    printf("Initialization complete. Waiting for presses...\n");

    // Main system loop
    while (true) {
        if (button_pressed_flag) {
            printf("Button was pressed! (Interrupt detected)\n");
            
            // Clear the flag after handling
            button_pressed_flag = false; 
        }
        
        // The CPU is free to do other tasks here instead of blocking!
        sleep_ms(10); 
    }

    return 0;
}
