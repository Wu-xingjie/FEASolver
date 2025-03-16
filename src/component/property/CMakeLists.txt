file(GLOB_RECURSE prop_FILE *.cpp)

add_library(prop SHARED ${prop_FILE})

#设置目标头文件目录
target_include_directories(prop PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(prop PUBLIC ${CMAKE_SOURCE_DIR}/tool)
