#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class Tri3 : public ElemBase {
public:
  Tri3();
  virtual ~Tri3() = default;

  virtual void SetComp(const file_data &datas) override;
  virtual elem_type ElemType() override { return ElemBase::elem_type::tri3; }
  virtual std::vector<int> GetNodes() override { return {_G1, _G2, _G3}; }
  virtual void GenerateK(const MODEL::Model &model) override;
  virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) override;

protected:
  // 计算三节点三角形单元形函数（面积坐标）的偏导数
  boost::shared_ptr<double>
  AreaCoordPartialDerivate(const vec_3 &n1, const vec_3 &n2, const char &lab);

private:
  int _pid;
  int _G1;
  int _G2;
  int _G3;
  Eigen::MatrixXd _loc_k;
};
} // namespace COMPONENT