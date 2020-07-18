cmake_minimum_required(VERSION 3.15)
project(algorithms_data_structures)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)
link_libraries(Catch2::Catch2)

add_executable(modular-test tests/modular-test.cc)

enable_testing()
add_test(modular modular-test)
