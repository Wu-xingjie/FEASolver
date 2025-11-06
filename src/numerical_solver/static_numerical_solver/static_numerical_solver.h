#pragma once

#include "numerical_solver_base.h"

namespace NUMERICAL {
class StaticNumSolver : public NumericalSolverBase {
public:
  StaticNumSolver(const MODEL::Model &model) : NumericalSolverBase(model){};
  virtual Eigen::VectorXd NumericalSolver() override;
};
} // namespace NUMERICAL