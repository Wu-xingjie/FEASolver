#include "delete_row_or_col.h"

namespace TOOL {
Eigen::MatrixXd DelRowOrCol(const RowOrCol& tap, const std::vector<int>& idxs,
                            const Eigen::MatrixXd& matrix) {
  Eigen::MatrixXd result = matrix;
  int col = result.cols();
  int row = result.rows();
  for (auto idx : idxs) {
    //   删除列
    if (tap == RowOrCol::col or tap == RowOrCol::both) {
      if (idx == 0) {
        result << result.rightCols(col - 1);
      } else if (idx == col - 1) {
        result << result.leftCols(col - 1);
      } else {
        result << result.leftCols(idx - 1), result.rightCols(col - idx);
      }
    }
    // 删除行
    if (tap == RowOrCol::row or tap == RowOrCol::both) {
      if (idx == 0) {
        result << result.rightCols(row - 1);
      } else if (idx == row - 1) {
        result << result.leftCols(row - 1);
      } else {
        result << result.leftCols(idx - 1), result.rightCols(row - idx);
      }
    }
  }
  return result;
}

Eigen::VectorXd DelRowOrCol(const std::vector<int>& idxs,
                            const Eigen::VectorXd& v) {
  Eigen::VectorXd result = v;
  int num = v.size();
  for (auto idx : idxs) {
    if (idx == 0) {
      result << result.tail(num - 1);
    } else if (idx == num - 1) {
      result << result.head(num - 1);
    } else {
      result << result.head(idx - 1), result.tail(num - idx);
    }
  }
  return result;
}
}  // namespace TOOL