#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class BAR : public ElemBase {
 public:
  BAR();
  virtual ~BAR() = default;

  virtual void SetComp(const file_data &datas) override;
  virtual elem_type ElemType() override { return ElemBase::elem_type::bar; }
  virtual std::vector<int> GetNodes() override { return {_G1, _G2}; }
  virtual void GenerateK(const MODEL::Model &model) override;
  virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) override;

 private:
  int _pid;
  int _G1;
  int _G2;
  Eigen::Vector3d _origin_vec;
  Eigen::MatrixXd _loc_k;
};
}  // namespace COMPONENT