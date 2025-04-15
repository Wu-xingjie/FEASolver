#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class ROD : public ElemBase {
 public:
  ROD() { _elem_type = ElemBase::elem_type::rod; };
  ~ROD() = default;

  virtual void SetComp(const file_data &datas) override;

  // 生成单元刚度矩阵
  virtual void GenerateK(const MODEL::Model &model) override;
  // 获取单元刚度矩阵
  Eigen::Matrix2d GetK();
  virtual elem_type ElemType() { return _elem_type; }

 private:
  // 杆单元节点
  int _G1;
  int _G2;
  // 杆单元属性
  int _pid;
  // 运用PETSC库中的矩阵来实现单元刚度矩阵
  Eigen::Matrix2d _loc_k;
};
}  // namespace COMPONENT