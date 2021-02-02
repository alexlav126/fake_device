cmake_minimum_required(VERSION 3.1)

project(proto)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES_LIB
  "proto.cpp"
  "command.cpp"
  "command_start_measure.cpp"
  "command_stop_measure.cpp"
  "command_set_range.cpp"
  "command_get_status.cpp"
  "command_get_result.cpp"
  )

add_library(${PROJECT_NAME} STATIC ${SOURCES_LIB})

