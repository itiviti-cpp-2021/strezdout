cmake_minimum_required(VERSION 3.13)

project(wordnet)
set(CMAKE_CXX_STANDARD 17)

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE SYSTEM ${COMMON_INCLUDES})

set(COMPILE_OPTS -O0)

if(EXISTS ${PROJECT_SOURCE_DIR}/src)
  file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
  list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
  add_library(wordnet_lib ${SRC_FILES})
  target_compile_options(wordnet_lib PUBLIC ${COMPILE_OPTS})

  add_executable(wordnet ${PROJECT_SOURCE_DIR}/wordnet.cpp)
  target_compile_options(wordnet PRIVATE ${COMPILE_OPTS})
  target_link_libraries(wordnet wordnet_lib)
else()
  add_executable(wordnet ${PROJECT_SOURCE_DIR}/wordnet.cpp)
  target_compile_options(wordnet PRIVATE ${COMPILE_OPTS})
  target_link_libraries(wordnet wordnet_lib)
endif()
