aux_source_directory(. DIR_ws2812_SRCS)
add_library(ws2812 ${DIR_ws2812_SRCS})
target_link_libraries(ws2812 PUBLIC hardware_pio pico_stdlib)
target_include_directories(ws2812 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/generated)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
