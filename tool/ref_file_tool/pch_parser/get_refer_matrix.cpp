#include "get_refer_matrix.h"

Eigen::MatrixXd TOOL::GetMatrix(const std::string &pch_path, const int &dim) {
  TOOL::PchParser parser(pch_path, dim);
  parser.read_file();
  return parser.get_matrix();
}