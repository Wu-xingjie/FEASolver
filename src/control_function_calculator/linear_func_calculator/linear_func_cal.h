#pragma once

#include "control_func_cal_base.h"

namespace NUMERICAL {
class LinearFuncCalculator : public ControlFuncCalBase {
public:
  LinearFuncCalculator(const MODEL::Model &model) : ControlFuncCalBase(model){};
  virtual Eigen::VectorXd NumericalSolver() override;
};
} // namespace NUMERICAL