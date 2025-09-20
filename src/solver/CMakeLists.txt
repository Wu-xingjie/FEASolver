message("************** solver start *************")
add_subdirectory(static_solver)
add_library(SolverBase SHARED solver_base.cpp)
message("************** solver end   *************")
