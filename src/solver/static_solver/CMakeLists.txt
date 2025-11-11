add_library(StaticSolver SHARED static_solver.cpp)
add_executable(static_solver main.cpp)

target_link_libraries(StaticSolver SolverBase)
target_include_directories(StaticSolver INTERFACE ${Eigen3_INC_DIRS})
target_include_directories(StaticSolver PUBLIC ${BOOST_INCLUDE_DIRS})
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src/model_mapper)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src/component/coordinate)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src/component/constrained)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src/component_factory)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src/FEAmodel)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src/matrix_assemble)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/tool)
target_include_directories(StaticSolver PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(static_solver StaticSolver pthread)

# 将可执行程序放入制定的目录下
set_target_properties(static_solver PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
message("${CMAKE_SOURCE_DIR}/bin")