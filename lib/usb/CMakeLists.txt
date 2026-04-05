# Find all source files in a single current directory
aux_source_directory(. DIR_usb_SRCS)

# Generate the link library
add_library(usb ${DIR_usb_SRCS})

include_directories(../flash)

target_link_libraries(usb PUBLIC
  pico_stdlib
  hardware_flash
  hardware_sync
  tinyusb_device
  tinyusb_board

  flash
)
