cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 23)
include(pico_sdk_import.cmake)
project(midi-pico)

pico_sdk_init()

add_executable(main
    main.cpp
)

target_link_libraries(main pico_stdlib)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)

pico_add_extra_outputs(main)