cmake_minimum_required(VERSION 3.13)

project(8puzzle)
set(CMAKE_CXX_STANDARD 17)

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE SYSTEM ${COMMON_INCLUDES})

set(COMPILE_OPTS -O3)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_library(8puzzle_lib ${SRC_FILES})
target_compile_options(8puzzle_lib PUBLIC ${COMPILE_OPTS})

add_executable(8puzzle ${PROJECT_SOURCE_DIR}/8puzzle.cpp)
target_compile_options(8puzzle PRIVATE ${COMPILE_OPTS})
target_link_libraries(8puzzle 8puzzle_lib)
