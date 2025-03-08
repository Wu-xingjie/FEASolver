cmake_minimum_required(VERSION 3.28)
project(FEASolver CXX)

# 添加子目录
add_subdirectory(src)
add_subdirectory(tool)
add_subdirectory(test)

set(CMAKE_BUILD_TYPE debug)