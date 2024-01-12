cmake_minimum_required(VERSION 3.28.1)

project(eva-vm)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic)

set(SOURCE_FILES src/eva-vm.cpp)

add_executable(eva-vm ${SOURCE_FILES})
