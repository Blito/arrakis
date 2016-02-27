project (arrakis)

cmake_minimum_required (VERSION 2.8.0)

# Build using C++14
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file (GLOB_RECURSE arrakis_SRC "src/*.cpp")

add_executable(arrakis ${arrakis_SRC})
# Include EntityX
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/entityx/cmake/")

find_package(EntityX REQUIRED)
if (ENTITYX_FOUND)
#    include_directories(ENTITYX_INCLUDE_DIR)
#    target_link_libraries(ENTITYX_LIBRARY)
    message("found")
else()
    message("Could not find EntityX automatically.")
    set (ENTITYX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/entityx/")
    set (ENTITYX_LIBRARY_DEBUG "")
    message(${ENTITYX_INCLUDE_DIR})
    message(${ENTITYX_LIBRARY_DEBUG})
endif()
include_directories("${ENTITYX_INCLUDE_DIR}")
target_link_libraries(arrakis "${ENTITYX_LIBRARY_DEBUG}")


#add_subdirectory(test)
#add_subdirectory(examples)
