# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(mReader C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()


# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    src/main.c
    src/ws2812/ws2812.c
    src/epaper/epaper.c
    src/epaper/EPD_1in54_V2.c
    src/epaper/DEV_Config.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    hardware_spi
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/generated
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/ws2812/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
