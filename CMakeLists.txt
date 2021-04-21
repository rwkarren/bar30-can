# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# assuming Pico SDK is in the same root dir as this project
set(PICO_SDK_PATH "../../pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(bar30-can C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(bar30-can main.cpp ms5837.cpp mcp2515.cpp)

pico_set_program_name(bar30-can "bar30-can")
pico_set_program_version(bar30-can "0.1")

pico_enable_stdio_uart(bar30-can 1)
pico_enable_stdio_usb(bar30-can 0)

# Add libraries
target_link_libraries(bar30-can pico_stdlib hardware_i2c hardware_spi)

pico_add_extra_outputs(bar30-can)

