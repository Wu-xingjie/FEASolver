file(GLOB_RECURSE constrain_FILE *.cpp)

add_library(constrain SHARED ${constrain_FILE})

#设置目标头文件目录
target_include_directories(constrain PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(constrain PUBLIC ${CMAKE_SOURCE_DIR}/tool)
