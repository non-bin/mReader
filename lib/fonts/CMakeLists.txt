aux_source_directory(. DIR_fonts_SRCS)

# Generate the link library
add_library(fonts ${DIR_fonts_SRCS})
