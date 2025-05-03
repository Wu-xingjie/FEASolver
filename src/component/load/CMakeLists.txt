file(GLOB_RECURSE load_FILE *.cpp)

add_library(load SHARED ${load_FILE})

#设置目标头文件目录
target_include_directories(load PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(load PUBLIC ${CMAKE_SOURCE_DIR}/tool)
