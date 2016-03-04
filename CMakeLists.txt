cmake_minimum_required(VERSION 3.3)
project(rpal)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp stackoftrees.cpp Lex.cpp Token.h Token.cpp Lex.h)
add_executable(rpal ${SOURCE_FILES})