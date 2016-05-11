cmake_minimum_required(VERSION 3.5)

project(kscope)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE TEST_FILES tests/*.cpp)

include_directories( /usr/local/include/ )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -g")

add_executable(tests ${SOURCE_FILES})

