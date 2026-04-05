set(FATFS_EXTERNAL_DIR $ENV{FATFS_PATH})
set(FATFS_COPY_DIR ${CMAKE_CURRENT_BINARY_DIR}/fatfs_copy)

# Copy the original sources at configure time, but NOT their ffconf.h
file(MAKE_DIRECTORY ${FATFS_COPY_DIR})
configure_file(${FATFS_EXTERNAL_DIR}/ff.c ${FATFS_COPY_DIR}/ff.c COPYONLY)
configure_file(${FATFS_EXTERNAL_DIR}/ffunicode.c ${FATFS_COPY_DIR}/ffunicode.c COPYONLY)
configure_file(${FATFS_EXTERNAL_DIR}/ff.h ${FATFS_COPY_DIR}/ff.h COPYONLY)
configure_file(${FATFS_EXTERNAL_DIR}/diskio.h ${FATFS_COPY_DIR}/diskio.h COPYONLY)

set(DIR_fatfs_SRCS
    ff_flash.c
    ${FATFS_COPY_DIR}/ff.c
    ${FATFS_COPY_DIR}/ffunicode.c
)

# Generate the link library
add_library(fatfs ${DIR_fatfs_SRCS})

target_include_directories(fatfs PUBLIC . ${FATFS_COPY_DIR})

include_directories(../usb)
include_directories(../error)
include_directories(../flash)

target_link_libraries(fatfs PUBLIC flash)
