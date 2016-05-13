cmake_minimum_required(VERSION 3.5)

project(kscope)

include_directories( /usr/local/include/ )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -g")

set(SOURCE_FILES 
  src/lexer.cpp
  src/parser.cpp
)
add_executable(kscope ${SOURCE_FILES})

set(LEXER_TEST_SOURCE_FILES 
  src/lexer.cpp
  tests/test_lexer.cpp
)
add_executable(test_lexer ${LEXER_TEST_SOURCE_FILES})

