cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mReader C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

execute_process(COMMAND xxd -i
  INPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/README.md
  OUTPUT_VARIABLE content
  COMMAND_ERROR_IS_FATAL ANY
)
set(content "unsigned char welcome_file[] = {\n${content}};")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/text_files/welcome.h "${content}")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/text_files)

include_directories(src)
add_executable(${PROJECT_NAME} src/main.c src/helpers.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE PICO_MALLOC_PANIC=0) # Don't panic if malloc fails

include_directories(lib/ws2812)
add_subdirectory(lib/ws2812)
target_link_libraries(${PROJECT_NAME} ws2812)

include_directories(lib/error)
add_subdirectory(lib/error)
target_link_libraries(${PROJECT_NAME} error)

include_directories(lib/fonts) # Include this directory when searching for #include files
add_subdirectory(lib/fonts) # Run the CMakeLists.txt in this directory
target_link_libraries(${PROJECT_NAME} fonts)

include_directories(lib/ePaper)
add_subdirectory(lib/ePaper)
target_link_libraries(${PROJECT_NAME} ePaper)

include_directories(lib/gui)
add_subdirectory(lib/gui)
target_link_libraries(${PROJECT_NAME} gui)

include_directories(lib/flash)
add_subdirectory(lib/flash)
target_link_libraries(${PROJECT_NAME} flash)

include_directories(lib/fatfs)
add_subdirectory(lib/fatfs)
target_link_libraries(${PROJECT_NAME} fatfs)

include_directories(lib/usb)
add_subdirectory(lib/usb)
target_link_libraries(${PROJECT_NAME} usb)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME}) # create map/bin/hex file etc.
