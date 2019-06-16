cmake_minimum_required(VERSION 3.14)
project(Huffman)

set(CMAKE_CXX_STANDARD 17)

add_library(huffman STATIC additional/additional.cpp additional/additional.hpp additional/consts/consts.hpp
        counter/counter.cpp counter/counter.cpp tree/tree.cpp tree/tree.hpp tree/bit_set/bit_set.hpp)
add_library(lib_file STATIC testing/file/myfile.cpp testing/file/myfile.hpp testing/file/reader.cpp testing/file/reader.hpp
        testing/file/writer.cpp testing/file/writer.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -Wall -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" -lpthread)
add_executable(huffman_tests
        testing/huff_testing.cpp
        testing/gtest/gtest-all.cc
        testing/gtest/gtest.h
        testing/gtest/gtest_main.cc
        )
target_link_libraries(huffman_tests pthread huffman lib_file)