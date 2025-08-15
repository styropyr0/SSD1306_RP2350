# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(SSD1306_RP2350 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(SSD1306_RP2350
    main.cpp
    pio_i2c.c        
    SSD1306.cpp
)


# Generate header from PIO assembly file
pico_generate_pio_header(SSD1306_RP2350 ${CMAKE_CURRENT_LIST_DIR}/i2c.pio)

# Include current dir so headers like "pio_i2c.h" are found
target_include_directories(SSD1306_RP2350 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link against required Pico SDK libs
target_link_libraries(SSD1306_RP2350
    pico_stdlib
    hardware_pio
    hardware_i2c
    pico_stdio_usb
)

# Enable USB stdio output, disable UART (optional)
pico_enable_stdio_usb(SSD1306_RP2350 1)
pico_enable_stdio_uart(SSD1306_RP2350 0)

# Create UF2, bin, and hex files
pico_add_extra_outputs(SSD1306_RP2350)
