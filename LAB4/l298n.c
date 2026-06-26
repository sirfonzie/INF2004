#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define GPIO pins
#define PWM_PIN 2     // GP2 for PWM
#define DIR_PIN1 0    // GP0 for direction
#define DIR_PIN2 1    // GP1 for direction

// Function to set up the PWM
void setup_pwm(uint gpio, float freq, float duty_cycle) {
    // Set the GPIO function to PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to the specified GPIO
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Calculate the PWM frequency and set the PWM wrap value
    float clock_freq = 125000000.0f;  // Default Pico clock frequency in Hz
    uint32_t divider = clock_freq / (freq * 65536);  // Compute divider for given frequency
    pwm_set_clkdiv(slice_num, divider);

    // Set the PWM wrap value (maximum count value)
    pwm_set_wrap(slice_num, 65535);  // 16-bit counter (0 - 65535)

    // Set the duty cycle
    pwm_set_gpio_level(gpio, (uint16_t)(duty_cycle * 65536));

    // Enable the PWM
    pwm_set_enabled(slice_num, true);
}

int main() {
    // Initialize GPIO pins for direction control
    gpio_init(DIR_PIN1);
    gpio_init(DIR_PIN2);
    gpio_set_dir(DIR_PIN1, GPIO_OUT);
    gpio_set_dir(DIR_PIN2, GPIO_OUT);

    // Set up PWM on GPIO0
    setup_pwm(PWM_PIN, 100.0f, 0.5f);  // 100 Hz frequency, 50% duty cycle

    // Control motor direction
    while (true) {
        // Forward direction
        gpio_put(DIR_PIN1, 1);
        gpio_put(DIR_PIN2, 0);
        sleep_ms(2000);

        // Reverse direction
        gpio_put(DIR_PIN1, 0);
        gpio_put(DIR_PIN2, 1);
        sleep_ms(2000);
    }

    return 0;
}
