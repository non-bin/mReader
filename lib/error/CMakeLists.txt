aux_source_directory(. DIR_error_SRCS)

include_directories(../ws2812)

# Generate the link library
add_library(error ${DIR_error_SRCS})
target_link_libraries(error PUBLIC pico_stdlib ws2812)
