message("-------------- assemble start -------------")
# TODO：暂缓刚度矩阵一维度变带宽存储的工作
# add_subdirectory(one_dim_varialbe_bandwidth_stroage)


add_library(matrix_assemble SHARED matrix_assemble.cpp)
target_include_directories(matrix_assemble PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(matrix_assemble PUBLIC ${CMAKE_SOURCE_DIR}/src/component/coordinate)
target_include_directories(matrix_assemble PUBLIC ${CMAKE_SOURCE_DIR}/tool)
target_include_directories(matrix_assemble PUBLIC ${BOOST_INCLUDE_DIRS})
target_link_libraries(matrix_assemble ${Boost_LIBS})
target_link_libraries(matrix_assemble tool)

message("-------------- assemble end   -------------")