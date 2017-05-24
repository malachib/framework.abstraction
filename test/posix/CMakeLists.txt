cmake_minimum_required(VERSION 3.7)
project(frab_test)

# temporarily just to get things online
#ADD_DEFINITIONS(-D__MBED__)

set(CMAKE_CXX_STANDARD 11)

file(GLOB framework_abstraction_source "framework.abstraction/frab/*.cpp")
add_library(framework_abstraction "${framework_abstraction_source}")
include_directories(framework.abstraction)
#//add_library()

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} framework_abstraction)