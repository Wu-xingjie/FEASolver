#pragma once
#include <eigen3/Eigen/Dense>
#include <map>
#include "fea_model/fea_model.h"
namespace ASSEMBLE {
class MatrixAssemble {
public:
  MatrixAssemble(const MODEL::Model &model);
  ~MatrixAssemble() = default;

  // 给控制方程施加约束
  void AddConstrain();

  // 组装总体刚度矩阵
  void AssembleK();

  // 组装总体载荷列阵
  void AssembleLoad();

  
  int _dof{0};                            // 模型自由度
  std::map<std::string, int> _dof2idx; // 总体坐标系自由度到维度的映射关系
  MODEL::Model _model;                 // 模型库
  Eigen::MatrixXd _matrix_k;           // 刚度矩阵
  Eigen::VectorXd _vector_f;           // 载荷列阵
};
} // namespace ASSEMBLE