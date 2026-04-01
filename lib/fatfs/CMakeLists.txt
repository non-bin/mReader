# Find all source files in a single current directory
# Save the name to DIR_fatfs_SRCS
aux_source_directory(. DIR_fatfs_SRCS)

# Generate the link library
add_library(fatfs ${DIR_fatfs_SRCS})

include_directories(../TinyUSB)
include_directories(../error)

target_link_libraries(fatfs PUBLIC hardware_flash)
