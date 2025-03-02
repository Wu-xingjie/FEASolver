cmake_minimum_required(VERSION 3.28)
project(tool CXX)

file(GLOB_RECURSE tool_cpp "*.cpp")

add_library(tool STATIC ${tool_cpp})