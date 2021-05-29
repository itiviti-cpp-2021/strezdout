cmake_minimum_required(VERSION 3.13)

project(2dtree)
set(CMAKE_CXX_STANDARD 17)

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE SYSTEM ${COMMON_INCLUDES})

set(COMPILE_OPTS -O3)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_library(2dtree_lib ${SRC_FILES})
target_compile_options(2dtree_lib PUBLIC ${COMPILE_OPTS})

add_executable(2dtree ${PROJECT_SOURCE_DIR}/2dtree.cpp)
target_compile_options(2dtree PRIVATE ${COMPILE_OPTS})
target_link_libraries(2dtree 2dtree_lib)

