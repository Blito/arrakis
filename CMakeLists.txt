project (arrakis)

cmake_minimum_required (VERSION 2.8.0)

# Build using C++14
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file (GLOB_RECURSE arrakis_SRC "src/*.cpp")

add_executable(arrakis ${arrakis_SRC})

add_subdirectory(test)
add_subdirectory(examples)