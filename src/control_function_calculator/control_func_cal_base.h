#pragma once
#include "matrix_assemble/one_dim_varialbe_bandwidth_stroage/one_dim_storage.h"
#include <boost/make_shared.hpp>

namespace NUMERICAL {
// 数值求解器基类
class ControlFuncCalBase {
public:
  ControlFuncCalBase(const MODEL::Model &model) {
    _assemble = boost::make_shared<ASSEMBLE::OneDimMatrixAssemble>(model);
    // 在数值求解器基类基类构造函数中组装刚度矩阵，目的是让数值求解器专注于数值求解
    _assemble->AssembleK();
  };
  ~ControlFuncCalBase() = default;

  virtual Eigen::VectorXd NumericalSolver() = 0;

public:
  boost::shared_ptr<ASSEMBLE::OneDimMatrixAssemble> _assemble;
};
} // namespace NUMERICAL