# Find all source files in a single current directory
# Save the name to DIR_gui_SRCS
aux_source_directory(. DIR_gui_SRCS)

include_directories(../fonts)

# Generate the link library
add_library(gui ${DIR_gui_SRCS})
target_link_libraries(gui PUBLIC fonts)
