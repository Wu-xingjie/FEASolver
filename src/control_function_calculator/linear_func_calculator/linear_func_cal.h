#pragma once

#include "control_func_cal_base.h"

namespace NUMERICAL {
// 基于cholesky分解求解线性控制方程Ku=F
class LinearFuncCalculator : public ControlFuncCalBase {
public:
  LinearFuncCalculator(const MODEL::Model &model) : ControlFuncCalBase(model){};
  virtual Eigen::VectorXd NumericalSolver() override;

protected:
ASSEMBLE::OneDimModel 
};
} // namespace NUMERICAL