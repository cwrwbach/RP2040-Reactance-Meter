cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()
add_executable(freq_counter
  freq_counter.c lcd_1602_i2c.c
)

# pull in common dependencies and additional pwm hardware support
target_link_libraries(freq_counter pico_stdlib hardware_pwm)



#pico_enable_stdio_usb(test 1)
#pico_enable_stdio_uart(test 1)
pico_add_extra_outputs(freq_counter)
target_link_libraries(freq_counter pico_stdlib hardware_i2c)



