cmake_minimum_required(VERSION 3.6)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_library(fuckitPass MODULE
    fuckit.cpp
)

# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(fuckitPass PRIVATE cxx_range_for cxx_auto_type)
