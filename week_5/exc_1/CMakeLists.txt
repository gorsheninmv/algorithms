cmake_minimum_required(VERSION 3.5)
project(1)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic")

set(IO_DIR /home/mg/projects/dev/io)

set(SOURCE_FILES main.cpp)

add_definitions(-DDEBUG)

link_directories(${IO_DIR})
include_directories(${IO_DIR})

add_executable(main ${SOURCE_FILES})

target_link_libraries(main io)



