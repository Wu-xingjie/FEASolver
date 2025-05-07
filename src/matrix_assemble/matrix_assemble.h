#pragma once
#include "fea_model/fea_model.h"
#include <eigen3/Eigen/Dense>
namespace ASSEMBLE {
class MatrixAssemble {
public:
  MatrixAssemble(const MODEL::Model &model);
  ~MatrixAssemble() = default;

  void AddConstrain(const MODEL::Model &model);
  void AssembleK(const MODEL::Model &model);
  void AssembleLoad(const MODEL::Model &model);

private:
  int _dof;                  // 模型自由度
  MODEL::Model _model;       // 模型库                  // 模型自由度数
  Eigen::MatrixXd _matrix_k; // 刚度矩阵
  Eigen::VectorXd _vector_f; // 载荷列阵
};
} // namespace ASSEMBLE