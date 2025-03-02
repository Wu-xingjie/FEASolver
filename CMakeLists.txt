cmake_minimum_required(VERSION 3.28)
project(tool CXX)

#设置全局变量
set(LibPath ${CMAKE_CURRENT_SOURCE_DIR})

# 设置头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加子目录
add_subdirectory(tool)
add_subdirectory(test)