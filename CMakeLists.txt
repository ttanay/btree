cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(btree VERSION 1.0)
add_executable(btree_test btree.cpp btree_test.cpp)
add_executable(btree_stringifier btree.cpp stringifier.cpp)
