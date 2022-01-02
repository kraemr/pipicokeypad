# pipicokeypad

How To Build On Linux:

Install CMake (at least version 3.13), and GCC cross compiler

`sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`

Set up your project to point to use the Raspberry Pi Pico SDK

    Either by cloning the SDK locally (most common) :

        git clone this Raspberry Pi Pico SDK repository

        Copy pico_sdk_import.cmake from the SDK into your project directory

        Set PICO_SDK_PATH to the SDK location in your environment, or pass it (-DPICO_SDK_PATH=) to cmake later.



First download the Pi Pico C/C++ SDK


