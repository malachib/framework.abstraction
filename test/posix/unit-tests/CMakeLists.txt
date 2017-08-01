cmake_minimum_required(VERSION 3.8)
project(unit_tests)

set(CMAKE_CXX_STANDARD 11)

set(FRAB_DIR "../../../src")
set(CATCH_DIR "../../../ext/Catch/include")

# TODO: consolidate this in a CMakeLists.txt
# +++
file(GLOB FRAB_SOURCE "${FRAB_DIR}/frab/*.cpp")
add_library(FRAB_LIB ${FRAB_SOURCE})
include_directories(${FRAB_DIR})
# ---

include_directories(${CATCH_DIR})

file(GLOB PROJECT_SOURCE "*.cpp")
set(SOURCE_FILES ${PROJECT_SOURCE})
add_executable(unit_tests ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} FRAB_LIB)
