cmake_minimum_required(VERSION 3.15)
project(algorithms_data_structures)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)
link_libraries(Catch2::Catch2)

add_executable(modular-test
        algebra/modular.hpp
        tests/modular-test.cc)
add_executable(stree-test
        data-structures/segment-tree/segment-tree.hpp
        data-structures/segment-tree/node-storage.hpp
        data-structures/segment-tree/walker.hpp
        tests/stree-test.cc)
add_executable(rmq-test
        data-structures/rmq.hpp
        tests/rmq-test.cc)

add_executable(bfs-test
        graphs/bfs.hpp
        tests/bfs-test.cc)

add_executable(aho-corasik-test
        strings/aho-corasick.hpp
        tests/aho-corasik-test.cc)

enable_testing()
add_test(modular modular-test)
add_test(stree stree-test)
add_test(rmq rmq-test)
add_test(bfs bfs-test)
add_test(aho-corasik aho-corasik-test)
