cmake_minimum_required(VERSION 3.0)
project(frab_test)

# temporarily just to get things online
#ADD_DEFINITIONS(-D__MBED__)

set(CMAKE_CXX_STANDARD 11)

file(GLOB framework_abstraction_source "framework.abstraction/frab/*.cpp")
add_library(framework_abstraction "${framework_abstraction_source}")
include_directories(framework.abstraction)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} framework_abstraction)

# Just so they'll appear in our IDE
file(GLOB_RECURSE framework_abstraction_headers "framework.abstraction/*.h*")
add_custom_target(headers SOURCES ${framework_abstraction_headers})
