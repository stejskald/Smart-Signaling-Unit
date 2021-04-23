# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# This component includes common functions for Ethernet communication
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/projects/smart_signaling_unit/components/eth_communication)

# This component includes implementation of Modbus TCP Server
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/projects/smart_signaling_unit/components/modbus_tcp_server)

# This component includes implementation of LED strip (WS2812B) controller
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/projects/smart_signaling_unit/components/led_strip_controller)

# This example uses an extra component for...
#list(APPEND EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/...)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(smart_signaling_unit)

#include_directories(include) # Setting the place, where are located header files (.h / .hpp)

#set(SRC # Variable SRC equals to the following source files (.c / .cpp)
#    main/smart_signaling_unit.c)

#add_executable(smart_signaling_unit ${SRC}) # Creates binary file named smart_signaling_unit, ${SRC} expands defined variable SRC

#target_link_libraries(smart_signaling_unit)