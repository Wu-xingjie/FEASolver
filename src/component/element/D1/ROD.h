#pragma once
#include "component/element/elemen_base.h"
#include "component/node/node.h"
#include <eigen3/Eigen/Core>

namespace ELEM {
class Rod : public ElemBase {
public:
  Rod() { _loc_k.setZero(); }
  ~Rod() = default;

  // 设置单元节点
  void NodeSet(const NODE::Node &g1, const NODE::Node &g2);
  // 生成单元刚度矩阵
  void GenerateK();

private:
  // 杆单元节点
  NODE::Node _G1;
  NODE::Node _G2;
  // 运用PETSC库中的矩阵来实现单元刚度矩阵
  Eigen::Matrix2d _loc_k;
};
} // namespace ELEM