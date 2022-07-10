cmake_minimum_required(VERSION 3.10)

project(btree VERSION 1.0)

include(CTest)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 REQUIRED)
find_package(benchmark REQUIRED)

add_executable(btree_stringifier stringifier.cpp)

add_executable(btree_test btree_test.cpp)
target_link_libraries(btree_test Catch2::Catch2)

add_executable(map_test map_test.cpp)
target_link_libraries(map_test Catch2::Catch2)

add_executable(btree_benchmark btree_benchmark.cpp)
target_link_libraries(btree_benchmark benchmark::benchmark)

# enable_testing()
# add_test(tests tests)
