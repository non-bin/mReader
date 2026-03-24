aux_source_directory(. DIR_ws2812_SRCS)
add_library(ws2812 ${DIR_ws2812_SRCS})
target_link_libraries(ws2812 PUBLIC hardware_pio pico_stdlib)
target_include_directories(ws2812 PRIVATE ${CMAKE_SOURCE_DIR}/generated)
