#include "delete_row_or_col.h"

#include <iostream>
#include <map>
namespace TOOL {
Eigen::MatrixXd DelRowOrCol(const RowOrCol& tap, const std::set<int>& idxs,
                            const Eigen::MatrixXd& matrix) {
  // 初始化结果矩阵
  int col = matrix.cols();
  int row = matrix.rows();
  int extro_dof_num = idxs.size();
  Eigen::MatrixXd result =
      Eigen::MatrixXd::Zero(row - extro_dof_num, col - extro_dof_num);
  // 创建原始矩阵自由度到结果矩阵自由度之间的映射
  std::map<int, int> origin_result_idx;
  int result_idx = 0;
  for (int origin_idx = 0; origin_idx < col; origin_idx++) {
    auto extro_idx_pos = idxs.find(origin_idx);
    if (extro_idx_pos != idxs.end()) {
      continue;
    }
    origin_result_idx[result_idx] = origin_idx;
    result_idx++;
  }
  // 给结果矩阵赋值
  for (int r = 0; r < row - extro_dof_num; r++) {
    for (int c = 0; c < row - extro_dof_num; c++) {
      result(r, c) = matrix(origin_result_idx.at(r), origin_result_idx.at(c));
    }
  }
  return result;
}

Eigen::VectorXd DelRowOrCol(const std::set<int>& idxs,
                            const Eigen::VectorXd& v) {
  // 初始化结果矩阵
  int v_size = v.size();
  int extro_dof_num = idxs.size();
  Eigen::VectorXd result = Eigen::VectorXd::Zero(v_size - extro_dof_num);
  // 创建原始矩阵自由度到结果矩阵自由度之间的映射
  std::map<int, int> origin_result_idx;
  int result_idx = 0;
  for (int origin_idx = 0; origin_idx < v_size; origin_idx++) {
    auto extro_idx_pos = idxs.find(origin_idx);
    if (extro_idx_pos != idxs.end()) {
      continue;
    }
    origin_result_idx[result_idx] = origin_idx;
    result_idx++;
  }
  // 给结果矩阵赋值
  for (int r = 0; r < v_size - extro_dof_num; r++) {
    result(r) = v(origin_result_idx.at(r));
  }
  return result;
}
}  // namespace TOOL