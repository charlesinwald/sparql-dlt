cmake_minimum_required(VERSION 2.8.12.2)
project(sparql_dlt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lcurl -pthread")


set(SOURCE_FILES pool.h main.cpp)
add_executable(sparql_dlt ${SOURCE_FILES})
target_link_libraries(sparql_dlt curl )
