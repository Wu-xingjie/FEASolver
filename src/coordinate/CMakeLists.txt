message("**************coordinate start**************")
file(GLOB_RECURSE COORDS *.cpp)

add_library(coord SHARED ${COORDS})

# 将coord库链接到libtool上
target_link_libraries(coord tool)
# 将头文件地址暴露出来
target_include_directories(coord PUBLIC ${CMAKE_SOURCE_DIR}/src/coordinate)
message("**************coordinate end**************")