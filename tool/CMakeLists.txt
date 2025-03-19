message("************** tool start **************")
# 将tool目录下的cpp文件打包成库
file(GLOB_RECURSE tls *.cpp)
add_library(tool SHARED ${tls})

# 连接库
target_link_libraries(tool node)

#设置目标头文件目录
target_include_directories(tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tool PUBLIC ${CMAKE_SOURCE_DIR}/src)
message("************** tool end   **************")