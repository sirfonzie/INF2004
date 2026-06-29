# Developing a Device Driver for Embedded Systems: Key Steps

This guide outlines essential steps for developing reliable and efficient device drivers for embedded systems, using the Raspberry Pi Pico (C/C++ SDK) as a reference. We will focus on a simple button driver with debouncing, progressing from basic concepts to best practices like interrupts.

## 1. Hardware Setup

Before writing code, ensure your hardware is wired correctly.
- **Microcontroller:** Raspberry Pi Pico
- **Component:** Push Button
- **Wiring:** Connect one side of the button to **GPIO Pin 2** and the other side to **GND**. 
*Note: We will configure an internal pull-up resistor in software, which means the pin will normally read HIGH (1), and will drop to LOW (0) when the button is pressed.*

## 2. Writing the Driver

### The Polling Approach (Basic)
The simplest way to check a button is to constantly read its state in a loop ("polling"). Because mechanical buttons "bounce" (generate multiple rapid signals on a single press), we need a **debounce** mechanism.

```c
#include "pico/stdlib.h"

#define BUTTON_PIN 2
#define DEBOUNCE_TIME_MS 50  // 50ms debounce window

uint32_t last_press_time = 0;

void button_init() {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Enable pull-up resistor
}

bool button_is_pressed() {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio_get(BUTTON_PIN) == 0) {  // Button pressed (pulled to GND)
        if (current_time - last_press_time > DEBOUNCE_TIME_MS) {
            last_press_time = current_time;
            return true;
        }
    }
    return false;
}
```

### The Interrupt Approach (Advanced & Recommended)
Polling wastes CPU cycles that could be used for other tasks. A better approach for device drivers is using **Interrupts (IRQs)**. The microcontroller will only run the button code when the hardware state actually changes.

Check the provided `button_driver.c` file for a complete, interrupt-driven implementation that sets a flag in the background when pressed.

## 3. Testing and Optimization

When testing drivers:
- **Print Statements:** Use `printf` to trace state changes, but avoid placing them inside interrupt handlers (ISRs), as they are slow and can cause crashes. Set a volatile flag instead and print in the main loop.
- **Power Efficiency:** Interrupts allow the microcontroller to sleep (`__wfi()`) while waiting for events, heavily optimizing power usage compared to polling.

## 4. Building the Project

To compile the provided `button_driver.c`, you need a proper build setup. 

1. Ensure you have the `pico-sdk` installed and the `PICO_SDK_PATH` environment variable set.
2. Copy `pico_sdk_import.cmake` from the SDK into this directory.
3. Use the provided `CMakeLists.txt` to build the code:

```bash
mkdir build
cd build
cmake ..
make
```
4. Flash the resulting `button_driver.uf2` file onto your Pico.
