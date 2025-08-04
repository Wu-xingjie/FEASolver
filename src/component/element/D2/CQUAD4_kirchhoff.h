#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class Cquad4Kf : public ElemBase {
public:
  Cquad4Kf();
  virtual ~Cquad4Kf() = default;

  virtual void SetComp(const file_data &datas) override;
  virtual elem_type ElemType() override { return ElemBase::elem_type::tri3; }
  virtual std::vector<int> GetNodes() override { return {_G1, _G2, _G3}; }
  virtual void GenerateK(const MODEL::Model &model) override;
  virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) override;

private:
// 
  double LenOfNode(const Eigen::Vector2d &n1, const Eigen::Vector2d &n2);

private:
  int _pid;
  int _G1;
  int _G2;
  int _G3;
  int _G4;
  Eigen::MatrixXd _loc_k;
};
} // namespace COMPONENT