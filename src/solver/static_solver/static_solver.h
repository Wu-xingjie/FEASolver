#pragma once

#include "solver_base.h"

namespace SOLVER {
// 结构静力学求解器
class StaticSolver : public SolverBase {
public:
  virtual std::map<std::string, double> solver() override;
  virtual void AssembleMatrix() override;
  

private:
  Eigen::MatrixXd _K;
  Eigen::VectorXd _f;
};
} // namespace SOLVER