cmake_minimum_required(VERSION 3.15)
project(algorithms_data_structures)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)
enable_testing()

function(add_catch name)
    add_executable("${name}-test" ${ARGN})
    add_test("${name}" "${name}-test")
endfunction()

function(add_catch_main name)
    add_catch(${name} ${ARGN})
    target_link_libraries("${name}-test" Catch2::Catch2WithMain)
endfunction()

include_directories(.)

add_catch_main(flat-graph   graphs/flat-graph.hpp   tests/flat-graph-test.cc)
add_catch_main(push-relabel graphs/push-relabel.hpp tests/push-relabel-test.cc)
add_catch_main(modular      algebra/modular.hpp     tests/modular-test.cc)
add_catch_main(rmq          data-structures/rmq.hpp tests/rmq-test.cc)
add_catch_main(stree
        data-structures/segment-tree/segment-tree.hpp
        data-structures/segment-tree/node-storage.hpp
        data-structures/segment-tree/walker.hpp
        tests/stree-test.cc)

add_catch(bfs         graphs/bfs.hpp      tests/bfs-test.cc)
add_catch(aho-corasik strings/aho-corasick.hpp tests/aho-corasik-test.cc)
