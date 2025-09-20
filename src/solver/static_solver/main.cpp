#include "static_solver.h"

int main(const int argc, const char *argv[]) {
  try {
    if (argc != 2) {
      throw std::runtime_error("[ERROR]:(main)>>>输入参数错误！");
    }
    std::string file_path = argv[1];
    SOLVER::StaticSolver static_solver;
    static_solver.GenCompLib(file_path);
    static_solver.AssembleMatrix();
    auto result = static_solver.solver();
    for (auto &elem : result) {
      std::cout << elem.first << "->" << elem.second << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}