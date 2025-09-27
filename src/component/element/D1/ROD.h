#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class ROD : public ElemBase {
public:
  ROD() {
    _elem_type = ElemBase::elem_type::rod;
    _loc_k = Eigen::MatrixXd::Zero(12, 12);
  };
  virtual ~ROD() = default;

  virtual void SetComp(const file_data &datas) override;

  // 生成单元刚度矩阵
  virtual void GenerateK(const MODEL::Model &model) override;
  // 获取单元刚度矩阵
  Eigen::MatrixXd GetK();
  virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) override;
  virtual elem_type ElemType() { return _elem_type; }
  std::vector<int> GetNodes() { return {_G1, _G2}; }

private:
  // 杆单元节点
  int _G1;
  int _G2;
  // 杆单元属性
  int _pid;
  // 局部坐标系下的单元刚度矩阵
  Eigen::MatrixXd _loc_k;
};
} // namespace COMPONENT