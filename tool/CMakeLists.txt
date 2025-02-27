cmake_minimum_required(VERSION 3.28)
project(tool CXX)

file(GLOB tool_cpp "*.cpp")

add_library(libtool STATIC ${tool_cpp})