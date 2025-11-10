#pragma once
#include "matrix_assemble/one_dim_varialbe_bandwidth_stroage/one_dim_storage.h"
#include <boost/make_shared.hpp>

namespace NUMERICAL {
// 数值求解器基类。
// 数值求解器之前应该完成刚度矩阵和载荷列阵的组装以及约束的施加工作
class ControlFuncCalBase {
public:
  ControlFuncCalBase(const MODEL::Model &model) {
    _assemble = boost::make_shared<ASSEMBLE::OneDimMatrixAssemble>(model);
  };
  ~ControlFuncCalBase() = default;
  // 进行数值求解
  virtual Eigen::VectorXd NumericalSolver() = 0;

protected:
  boost::shared_ptr<ASSEMBLE::OneDimMatrixAssemble> _assemble;
};
} // namespace NUMERICAL