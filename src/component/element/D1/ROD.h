#pragma once
#include "elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class ROD : public ElemBase {
public:
  ROD() : ElemBase() { _loc_k.setZero(); }
  ~ROD() = default;

  // 设置单元节点
  void SetNode(const int &g1, const int &g2);
  // 设置单元属性
  void SetProperty(const int &p);
  // 生成单元刚度矩阵
  void GenerateK(const MODEL::Model& model);
  // 获取单元刚度矩阵
  Eigen::Matrix2d GetK();

private:
  // 杆单元节点
  int _G1;
  int _G2;
  // 杆单元属性
  int _property;
  // 运用PETSC库中的矩阵来实现单元刚度矩阵
  Eigen::Matrix2d _loc_k;
};
} // namespace COMPONENT