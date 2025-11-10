#include "linear_func_cal.h"

namespace NUMERICAL {
Eigen::VectorXd LinearFuncCalculator::NumericalSolver() {
  Eigen::VectorXd modal_disp = Eigen::VectorXd::Zero(_assemble->GetModalDof());
  // 计算下三角矩阵
  
  // 第一次求解Ly=F
  // 第二次求解L^T*u=y

  return modal_disp;
}
} // namespace NUMERICAL