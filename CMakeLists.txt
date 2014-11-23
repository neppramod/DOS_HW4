cmake_minimum_required(VERSION 2.8.4)
project(HW4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-lpthread")
set(SOURCE_FILES main.cpp communication.cpp content.cpp)
add_executable(HW4 ${SOURCE_FILES})

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/portnums
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/neighbors
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/content
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)



