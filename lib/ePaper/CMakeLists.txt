aux_source_directory(. DIR_ePaper_SRCS)
include_directories(../Fonts)

# Generate the link library
add_library(ePaper ${DIR_ePaper_SRCS})
target_link_libraries(ePaper PUBLIC pico_stdlib hardware_spi)
