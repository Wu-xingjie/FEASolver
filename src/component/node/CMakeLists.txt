file(GLOB_RECURSE NODE_FILE *.cpp)

add_library(node SHARED ${NODE_FILE})

#设置目标头文件目录
target_include_directories(node PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(node PUBLIC ${CMAKE_SOURCE_DIR}/tool)
