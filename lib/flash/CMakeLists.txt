# Find all source files in a single current directory
# Save the name to DIR_fatfs_SRCS
# set(PICO_MALLOC_PANIC 0)

aux_source_directory(. DIR_flash_SRCS)

include_directories(../../src)

# Generate the link library
add_library(flash ${DIR_flash_SRCS})
# target_compile_definitions(${PROJECT_NAME} PUBLIC PICO_MALLOC_PANIC=0)

target_link_libraries(flash PUBLIC hardware_flash pico_stdlib)
