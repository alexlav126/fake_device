cmake_minimum_required(VERSION 3.1)

project(server)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES_EXE
  "server_app.cpp"
  "server.cpp"
  "meas_channel.cpp"
  "logger.cpp")

add_executable(${PROJECT_NAME} ${SOURCES_EXE})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads proto)