aux_source_directory(. DIR_ws2812_SRCS)

# Generate a header file from the PIO assembly file
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

add_library(ws2812 ${DIR_ws2812_SRCS})
target_link_libraries(ws2812 PUBLIC hardware_pio pico_stdlib)
target_include_directories(ws2812 PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
