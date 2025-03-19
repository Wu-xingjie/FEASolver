message("-------------- coordinate start -------------")
file(GLOB_RECURSE COORDS *.cpp)

add_library(coord SHARED ${COORDS})

# 将coord库链接到libtool上
# target_link_libraries(coord tool)
#设置目标头文件目录
target_include_directories(coord PUBLIC ${CMAKE_SOURCE_DIR}/src/coordinate)
target_include_directories(coord PUBLIC ${CMAKE_SOURCE_DIR}/tool)
message("-------------- coordinate end   -------------")