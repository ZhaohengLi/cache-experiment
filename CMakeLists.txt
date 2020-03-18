cmake_minimum_required(VERSION 3.5)
project(CacheSim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

aux_source_directory(src SOURCE_FILES)
add_executable(CacheSim ${SOURCE_FILES})
