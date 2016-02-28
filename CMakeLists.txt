project (arrakis)

cmake_minimum_required (VERSION 3.1.0)

# Build using C++14
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file (GLOB_RECURSE arrakis_SRC "src/*.cpp")
add_executable(arrakis ${arrakis_SRC})

# EntityX
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/entityx/cmake/")

find_package(EntityX REQUIRED)
if (NOT ENTITYX_FOUND)
    message("Could not find EntityX automatically.")
    set (ENTITYX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/entityx/")
    set (ENTITYX_LIBRARY_DEBUG "${CMAKE_SOURCE_DIR}/lib/entityx/build")
endif()
include_directories("${ENTITYX_INCLUDE_DIR}")
target_link_libraries(arrakis "${ENTITYX_LIBRARY_DEBUG}")

# websocketpp
set (WEBSOCKETPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/websocketpp/" CACHE PATH "Base websocketpp directory.")
include_directories(${WEBSOCKETPP_INCLUDE_DIR})

# Threads
find_package(Threads REQUIRED)
target_link_libraries(arrakis Threads::Threads)

# ASIO
add_definitions("-DASIO_STANDALONE")
set (ASIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/asio/asio/include/" CACHE PATH "Base ASIO directory (NOT Boost.ASIO).")
include_directories(${ASIO_INCLUDE_DIR})
