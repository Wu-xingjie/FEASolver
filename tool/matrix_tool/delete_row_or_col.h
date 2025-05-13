#pragma once
#include <eigen3/Eigen/Dense>
#include <set>

namespace TOOL {
enum class RowOrCol { row, col, both };

// 根据输入删除矩阵中行号或列号为idx的行或列
Eigen::MatrixXd DelRowOrCol(const RowOrCol& tap, const std::set<int>& idxs,
                            const Eigen::MatrixXd& matrix);

Eigen::VectorXd DelRowOrCol(const std::set<int>& idxs,
                            const Eigen::VectorXd& v);
}  // namespace TOOL