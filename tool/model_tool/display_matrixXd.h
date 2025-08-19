#pragma once
#include <eigen3/Eigen/Dense>
#include <string>
namespace TOOL {
// 打印矩阵
void DisplayMatrixXd(const Eigen::MatrixXd &matrix,
                     const std::string &title = "",
                     const bool &show_index = false);
} // namespace TOOL