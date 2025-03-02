cmake_minimum_required(VERSION 3.28)
project(test CXX)


##编译测试案例中的可执行程序
# 找到Tool中所有的文件
file(GLOB test_list "*.cpp")

# 添加库文件
link_libraries(${CMAKE_SOURCE_DIR}/tool)

# 遍历每一个cpp文件
foreach(test_elem ${test_list})
    get_filename_component(elem_name ${test_elem} NAME_WE)
    message(${test_elem})
    add_executable(${elem_name} ${test_elem})
    target_link_libraries(${elem_name} libtool)  
endforeach()