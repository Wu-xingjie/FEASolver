#pragma once
#include <array>
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>

namespace TOOL {
// 解析nastran生成的pch文件，得到模型约束后的总体刚度矩阵
class PchParser {
public:
  using pch_line = std::array<std::string, 6>;
  PchParser(const std::string &path, const int &dim)
      : _pch_path(path), _matrix_dim(dim) {}
  ~PchParser() = default;

  void read_file();
  Eigen::MatrixXd get_matrix();

private:
  std::string _pch_path;
  int _matrix_dim;
  std::vector<pch_line> _pch_content;
};
} // namespace TOOL