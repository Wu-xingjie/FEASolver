#pragma once
#include <eigen3/Eigen/Dense>

#include "fea_model/fea_model.h"
namespace ASSEMBLE {
class MatrixAssemble {
 public:
  MatrixAssemble(const MODEL::Model &model);
  ~MatrixAssemble() = default;

  void AddConstrain();
  void AssembleK();
  void AssembleLoad();

 private:
  int _dof;                             // 模型自由度
  std::map<std::string, int> _dof2idx;  // 总体坐标系自由度到维度的映射关系
  MODEL::Model _model;                  // 模型库
  Eigen::MatrixXd _matrix_k;  // 刚度矩阵
  Eigen::VectorXd _vector_f;  // 载荷列阵
};
}  // namespace ASSEMBLE