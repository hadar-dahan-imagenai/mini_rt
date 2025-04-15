#cmake_minimum_required(VERSION 3.10)
#project(rt_curves_lib)
#
## Set C++ standard
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Force platform architecture for MSVC (64-bit)
#if(MSVC)
#    message(STATUS "Using MSVC compiler")
#    #set(CMAKE_GENERATOR_PLATFORM x64)  # Target 64-bit architecture
#    message(STATUS "Setting generator platform to x64 (if targeting 64-bit architecture)")
#endif()
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse2")
#
## Define include directories
#include_directories(${CMAKE_SOURCE_DIR}/include)
#
## Add the source files
#set(SOURCES
#        src/curves.cpp
#      #  src/ciecam02.cc
#        src/color.cc
#        src/diagonalcurves.cc
#        src/main.cpp
#        #  src/iccstore.cc
#
#)
#
## Define the library target
#add_library(rt_curves_lib ${SOURCES})
#
## Link libraries if necessary
## target_link_libraries(rt_curves_lib glibmm-2.4 glib-2.0)
cmake_minimum_required(VERSION 3.10)
project(rt_curves_lib)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force platform architecture for MSVC (64-bit)
if(MSVC)
    message(STATUS "Using MSVC compiler")
    message(STATUS "Setting generator platform to x64 (if targeting 64-bit architecture)")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse2")

# Define include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files for the library (excluding main.cpp)
set(LIB_SOURCES
        src/curves.cpp
        src/color.cc
        src/diagonalcurves.cc
)

# Define the library target
add_library(rt_curves_lib ${LIB_SOURCES})

# Now add the executable
add_executable(rt_curves_app src/main.cpp)

# Link the executable with the library
target_link_libraries(rt_curves_app rt_curves_lib)
