cmake_minimum_required(VERSION 3.8)
project(unit_tests)

set(CMAKE_CXX_STANDARD 11)

set(FRAB_DIR "../../../src")

# TODO: consolidate this in a CMakeLists.txt
# +++
file(GLOB FRAB_SOURCE "${FRAB_DIR}/frab/*.cpp")
add_library(FRAB_LIB ${FRAB_SOURCE})
include_directories(${FRAB_DIR})
# ---

set(SOURCE_FILES main.cpp)
add_executable(unit_tests ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} FRAB_LIB)
