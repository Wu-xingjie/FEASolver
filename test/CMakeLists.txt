enable_testing()
message("**************test start**************")

# 遍历test下的每一个cpp文件
file(GLOB_RECURSE test_lists *.cpp)
foreach(elem_test ${test_lists})
    message(">>>>elem test is ${elem_test}")
    # 获取cpp文件名
    get_filename_component(elem_name ${elem_test} NAME_WE)
    message(">>>>elem name is ${elem_name}")

    # 获取cpp文件所在目录名
    get_filename_component(elem_address ${elem_test} DIRECTORY)
    message(">>>>elem_address is ${elem_address}")
    get_filename_component(elem_dir ${elem_address} NAME_WE)
    message(">>>>${elem_name} is at ${elem_dir}")

    # 创建可执行程序
    add_executable(${elem_name} ${elem_test})
    # 连接依赖
    target_link_libraries(${elem_name} tool)
    target_include_directories(${elem_name} PUBLIC ${CMAKE_SOURCE_DIR}/tool)

    # 创建可执行程序存放目录
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/${elem_dir})
    # 将可执行程序放入制定的目录下
    message("new directory: ${CMAKE_SOURCE_DIR}/build/test/${elem_dir}")
    set_target_properties(${elem_name} PROPERTIES
                             RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${elem_dir})
endforeach()

message("**************test end**************")
