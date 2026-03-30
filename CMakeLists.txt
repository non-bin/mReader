# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

# Set name of project (as PROJECT_NAME) and C/C++ Standard
project(mReader C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Treat flash as only 1MB for the firmware link, leaving the upper 1MB unused
set(PICO_FLASH_SIZE_BYTES 1048576)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# add a header directory
include_directories(src)
include_directories(./lib/gui)
include_directories(./lib/ePaper)
include_directories(./lib/ws2812)
include_directories(./lib/TinyUSB)

# add a compilation subdirectory
add_subdirectory(lib/ePaper)
add_subdirectory(lib/fonts)
add_subdirectory(lib/gui)
add_subdirectory(lib/ws2812)
add_subdirectory(lib/TinyUSB)


# target_sources(${PROJECT_NAME}

add_executable(${PROJECT_NAME}
    src/main.c
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    hardware_spi

    TinyUSB

    ePaper
    gui
    fonts

    ws2812
)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/generated
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/lib/ws2812/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
