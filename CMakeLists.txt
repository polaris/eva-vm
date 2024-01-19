cmake_minimum_required(VERSION 3.28.1)

project(eva-vm)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic -ggdb3)

set(SOURCE_FILES src/vm/EvaVM.cpp src/vm/EvaCompiler.cpp src/vm/EvaParser.cpp)

add_executable(eva-vm src/eva-vm.cpp ${SOURCE_FILES})

find_package(Catch2 3 REQUIRED)

add_executable(tests tests/EvaVM_tests.cpp tests/EvaValue_tests.cpp ${SOURCE_FILES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
