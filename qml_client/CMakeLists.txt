cmake_minimum_required(VERSION 3.1)

project(qml_client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(SOURCES_EXE
  "main.cpp"
  "channelhandler.cpp"
  "qml.qrc")



if(NOT EXISTS proto)
set(SOURCES_LIB
  "../proto/proto.cpp"
  "../proto/command.cpp"
  "../proto/command_start_measure.cpp"
  "../proto/command_stop_measure.cpp"
  "../proto/command_set_range.cpp"
  "../proto/command_get_status.cpp"
  "../proto/command_get_result.cpp"
  )
add_library(proto STATIC ${SOURCES_LIB})
endif()

add_executable(${PROJECT_NAME} ${SOURCES_EXE})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick proto)
