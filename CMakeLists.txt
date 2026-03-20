cmake_minimum_required(VERSION 3.16)

project(utg VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(utg 
    src/main.cpp
)

target_link_libraries(utg PRIVATE SDL3::SDL3)