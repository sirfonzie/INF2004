cmake_minimum_required(VERSION 3.13)

# Initialize the SDK
include(pico_sdk_import.cmake)

project(button_driver_example C CXX ASM)

pico_sdk_init()

# Define the executable
add_executable(button_driver
    button_driver.c
)

# Pull in pico_stdlib which provides standard IO, sleep functions, etc.
target_link_libraries(button_driver pico_stdlib)

# Enable USB standard output (for printf)
pico_enable_stdio_usb(button_driver 1)
pico_enable_stdio_uart(button_driver 0)

# Create map, bin, hex, and uf2 files
pico_add_extra_outputs(button_driver)
