#pragma once

#include "fea_model/fea_model.h"
#include "matrix_assemble/matrix_assemble.h"
#include "model_mapper/mapper.h"

namespace SOLVER {
// 求解器基类
class SolverBase {
public:
  SolverBase() = default;
  ~SolverBase() = default;
  // 创建元件库
  virtual void GenCompLib(const std::string &file_path);
  // 组装矩阵
  virtual void AssembleMatrix() = 0;
  // 计算输入文件，返回节点位移
  virtual std::map<std::string, double> solver() = 0;

protected:
  MODEL::Model _model;
  std::vector<std::string> _valid_dof;
};
} // namespace SOLVER