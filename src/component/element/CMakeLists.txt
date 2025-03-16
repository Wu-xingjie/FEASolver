file(GLOB_RECURSE ELEM_FILE *.cpp)

add_library(elem SHARED ${ELEM_FILE})

#设置目标头文件目录
target_include_directories(elem PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(elem PUBLIC ${CMAKE_SOURCE_DIR}/tool)
