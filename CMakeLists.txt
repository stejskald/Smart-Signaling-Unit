# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

# This component includes common functions for Ethernet communication
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/projects/smart_signaling_unit/components/eth_comm)

# This component includes usage of Modbus TCP Server
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/projects/smart_signaling_unit/components/mb_tcp_server)

# This component includes implementation of LED strip (WS2812B) display controller
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/projects/smart_signaling_unit/components/led_strip_display_ctrl)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(smart_signaling_unit)