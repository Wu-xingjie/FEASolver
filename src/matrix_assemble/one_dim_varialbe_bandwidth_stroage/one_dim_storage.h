#pragma once
#include <array>
#include <boost/bimap.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <map>
#include <string>
#include <vector>

#include "fea_model/fea_model.h"

namespace ASSEMBLE {
// 定义一维存储统一数据模型
// _none_zero_elem: 一维存储的刚度矩阵值
// _diag_elem_loc： 主对角线元素在_none_zero_elem中的位置
// _dof2idx： 自由度坐标和刚度矩阵元素之间的双向映射表
struct OneDimModel {
  Eigen::VectorXd _none_zero_elem;
  std::vector<int> _diag_elem_loc;
  boost::bimap<std::array<std::string, 2>, int> _dof2idx;
};

class OneDimMatrixAssemble {
public:
  OneDimMatrixAssemble(const MODEL::Model &model);
  ~OneDimMatrixAssemble() = default;

  // 组装刚度矩阵
  void AssembleK();
  void ShowK();

protected:
  std::vector<int> FindNodesOfDof(const std::string &dof);
  boost::shared_ptr<double> GetMatrixElem(const int &row, const int &col);

private:
  int _dof{0}; // 模型自由度
  boost::bimap<std::string, int>
      _dof2idx;        // 总体坐标系(全)自由度到维度的映射关系
  MODEL::Model _model; // 模型库
  std::map<std::string, std::vector<int>>
      _dof2node; // 记录模型中和某个自由度关联的所有单元的节点集
  OneDimModel _matrix_k;     // 刚度矩阵
  Eigen::VectorXd _vector_f; // 载荷列阵
};

} // namespace ASSEMBLE