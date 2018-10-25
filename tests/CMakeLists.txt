cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 14)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

add_executable(huffman_testing
        huffman_testing.cpp
        ../lib/Encryptor.cpp ../lib/Decryptor.cpp ../lib/Decryptor.h
        gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc ../app/FileCompressor.cpp ../app/FileCompressor.h)
target_link_libraries(huffman_testing -lpthread)

target_link_libraries(huffman_testing huffman_library)