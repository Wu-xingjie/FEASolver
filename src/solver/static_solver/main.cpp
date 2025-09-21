#include "static_solver.h"
#include <fstream>

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
    // 处理输出文件path
    auto idx = file_path.rfind("/");
    if (idx == std::string::npos) {
      throw std::runtime_error("[ERROR]:(main)>>>计算文件绝对路径异常！");
    }
    std::string file_dir = file_path.substr(0, idx);
    std::string output_file = file_dir + "/output.txt";
    std::cout << "计算结果文件路径:" << output_file << std::endl;

    // 输出计算结果
    std::fstream result_file;
    result_file.open(output_file, std::ios_base::out);
    if (result_file.is_open()) {
      for (auto &elem : result) {
        result_file << elem.first << "->" << elem.second << std::endl;
      }
    } else {
      throw std::runtime_error("[ERROR]:(main)>>>输出文件无法打开！");
    }

    result_file.close();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}