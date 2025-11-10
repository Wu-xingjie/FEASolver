#pragma once
#include "fea_model/fea_model.h"
#include <array>
#include <boost/bimap.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <map>
#include <string>
#include <vector>

namespace ASSEMBLE {
// 定义矩阵一维存储统一数据模型
// _none_zero_elem: 一维存储的刚度矩阵值
// _diag_elem_loc： 主对角线元素在_none_zero_elem中的位置
// _dof2idx： 自由度坐标和刚度矩阵元素之间的双向映射表
struct OneDimModel {
  Eigen::VectorXd _none_zero_elem;
  std::vector<int> _diag_elem_loc;
  boost::bimap<std::array<std::string, 2>, int> _dof2idx;

  // 矩阵加法
  boost::shared_ptr<OneDimModel> operator+(const OneDimModel &m);
};

class OneDimMatrixAssemble {
public:
  OneDimMatrixAssemble(const MODEL::Model &model);
  ~OneDimMatrixAssemble() = default;

  // 组装刚度矩阵
  void AssembleK();
  // 组装载荷列阵
  void AssembleF();
  // 打印刚度矩阵
  void ShowK();
  // 获取总体刚度矩阵元素值
  boost::shared_ptr<double> GetKElem(const int &row, const int &col);
  // 获取载荷列阵元素值
  boost::shared_ptr<double> GetLoadElem(const int &idx);
  // 获取模型自由度
  int GetModalDof() { return _dof; }

protected:
  std::vector<int> FindNodesOfDof(const std::string &dof);

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