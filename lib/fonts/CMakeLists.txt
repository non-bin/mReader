# Find all source files in a single current directory
# Save the name to DIR_fonts_SRCS
aux_source_directory(. DIR_fonts_SRCS)

# Generate the link library
add_library(fonts ${DIR_fonts_SRCS})
