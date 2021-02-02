cmake_minimum_required(VERSION 3.1)

project(client)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES_EXE "client.cpp")

add_executable(${PROJECT_NAME} ${SOURCES_EXE})