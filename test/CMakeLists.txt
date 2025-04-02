enable_testing()
message("************** test start **************")

# 遍历test下的每一个cpp文件
file(GLOB_RECURSE test_lists *.cpp)
foreach(elem_test ${test_lists})
    # 获取cpp文件名
    get_filename_component(elem_name ${elem_test} NAME_WE)

    # 获取cpp文件所在目录名
    get_filename_component(elem_address ${elem_test} DIRECTORY)
    get_filename_component(elem_dir ${elem_address} NAME_WE)

    # 创建可执行程序
    add_executable(${elem_name} ${elem_test})
    # 连接依赖
    target_link_libraries(${elem_name} tool)
    target_link_libraries(${elem_name} coord)
    target_link_libraries(${elem_name} node)
    target_link_libraries(${elem_name} elem)
    target_link_libraries(${elem_name} mat)
    target_link_libraries(${elem_name} prop)
    target_link_libraries(${elem_name} mapper)
    target_link_libraries(${elem_name} factory)

    target_link_libraries(${elem_name} ${BOOST_LIBS})


    target_include_directories(${elem_name} INTERFACE ${Eigen3_INC_DIRS})
    target_include_directories(${elem_name} PUBLIC ${BOOST_INCLUDE_DIRS})
    target_include_directories(${elem_name} PUBLIC ${CMAKE_SOURCE_DIR}/src/model_mapper)
    target_include_directories(${elem_name} PUBLIC ${CMAKE_SOURCE_DIR}/src/component/coordinate)
    target_include_directories(${elem_name} PUBLIC ${CMAKE_SOURCE_DIR}/src/component_factory)

    # 创建可执行程序存放目录
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/${elem_dir})
    # 将可执行程序放入制定的目录下
    set_target_properties(${elem_name} PROPERTIES
                             RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${elem_dir})
    set(CMAKE_BUILD_TYPE debug)
endforeach()
message("************** test end   **************")
