# pipicokeypad

What is this?

Its just a small personal project where i used the pico sdk and the tusb-library to make the pi pico send keypresses over the usb-port whenever the corresponding buttons are pressed .

How To Build On Linux:

Install CMake (at least version 3.13), and GCC cross compiler

`sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`

Set up your project to point to use the Raspberry Pi Pico SDK

Either by cloning the SDK locally (most common) :

`git clone https://github.com/raspberrypi/pico-sdk`

Copy pico_sdk_import.cmake from the SDK into your project directory

Set PICO_SDK_PATH to the SDK location in your environment, or pass it (-DPICO_SDK_PATH=) to cmake later.
Use This command in your project folder
`export PICO_SDK_PATH =PATH_TO_THE_SDK` 

Then we will need a CMakeLists.txt. mine should work just fine
cd into the build folder 
`cd build/`
Then run:
`cmake ..` 
After that is completed  run 
`make`
This will create the needed .uf2 File and some other files like .hex .bin and so on

If we are done with all of this, then just hold bootsel on your pi pico and plug it into your pc.
It should how up as a drive, onto which the .uf2 file should be dropped.
(Also Keep in Mind that it wont show up, if you have a micro usb cable, that does not support data)

