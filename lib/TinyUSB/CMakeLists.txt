# Find all source files in a single current directory
aux_source_directory(. DIR_TinyUSB_SRCS)

# Generate the link library
add_library(TinyUSB ${DIR_TinyUSB_SRCS})

target_link_libraries(TinyUSB PUBLIC
  pico_stdlib
  hardware_spi
  hardware_flash
  tinyusb_device
  tinyusb_board
)
