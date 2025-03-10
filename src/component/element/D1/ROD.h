#pragma once
#include <eigen3/Eigen/Core>

#include "element/elemen_base.h"
#include "node/node.h"
#include "property/PROD.h"

namespace COMPONENT {
class ROD : public ElemBase {
 public:
  ROD() { _loc_k.setZero(); }
  ~ROD() = default;

  // 设置单元节点
  void SetNode(const Node &g1, const Node &g2);
  // 设置单元属性
  void SetProperty(const PROD &p);
  // 生成单元刚度矩阵
  void GenerateK();

 private:
  // 杆单元节点
  Node _G1;
  Node _G2;
  // 杆单元属性
  PROD _property;
  // 运用PETSC库中的矩阵来实现单元刚度矩阵
  Eigen::Matrix2d _loc_k;
};
}  // namespace COMPONENT