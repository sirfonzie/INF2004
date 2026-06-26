add_executable(uart_rx uart_rx.c)

# pull in common dependencies
target_link_libraries(uart_rx pico_stdlib)

# enable usb output, enable uart output
pico_enable_stdio_usb(uart_rx 1)
pico_enable_stdio_uart(uart_rx 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_rx)

# add url via pico_set_program_url
example_auto_set_url(uart_rx)
