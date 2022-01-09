cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
project(main C CXX ASM)
set(CMAKE_C_STANDARD 11) 
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

add_executable(main main.c)

target_sources(main PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

target_include_directories(main PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(main PUBLIC pico_stdlib tinyusb_device tinyusb_board hardware_adc)
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
# create map/bin / hex 
pico_add_extra_outputs(main)
