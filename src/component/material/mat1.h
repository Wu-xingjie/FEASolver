#pragma once

#include <boost/optional.hpp>

#include "material_base.h"

namespace COMPONENT {
// 各项同性材料

struct Mat1Data {
  Mat1Data() = default;
  Mat1Data(const Mat1Data &p);
  boost::optional<double> _E;
  boost::optional<double> _NU;
  boost::optional<double> _G;
};

class MAT1 : public MaterialBase {
 public:
  MAT1() : MaterialBase() { _type = comp_type::mat; }
  ~MAT1() = default;
  MAT1(const MAT1 &p);

  // 设置各向同性材料数据(没有的数据项就传-1)
  virtual void SetComp(const file_data &datas) override;

  // 获取各向同性材料数据
  Mat1Data GetMaterial();
  virtual int GetID() override { return _id; }
  virtual comp_type Type() override { return _type; }

 private:
  Mat1Data _mat1_data;
};
}  // namespace COMPONENT