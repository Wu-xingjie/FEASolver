file(GLOB_RECURSE mat_file *.cpp)

add_library(mat SHARED ${mat_file})

#设置目标头文件目录
target_include_directories(mat PUBLIC ${CMAKE_SOURCE_DIR}/tool)
target_include_directories(mat PUBLIC ${CMAKE_SOURCE_DIR}/src)
