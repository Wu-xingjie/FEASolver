#pragma once

#include "material_base.h"

namespace COMPONENT {
// 各项同性材料

struct Mat1Data {
  Mat1Data(double E, double G, double NU) : _E(E), _G(G), _NU(NU) {}
  Mat1Data() : _E(-1.0), _G(-1.0), _NU(-1.0) {}
  Mat1Data(const Mat1Data &p);
  double _E;  // 杨氏模量
  double _G;  // 剪切模量
  double _NU; // 泊松比
};

class MAT1 : public MaterialBase {
public:
  MAT1() : MaterialBase() {}
  ~MAT1() = default;
  MAT1(const MAT1 &p);

  // 设置各向同性材料数据(没有的数据项就传-1)
  virtual void SetComp(const file_data &datas) override {}

  // 获取各向同性材料数据
  Mat1Data GetMaterial();
  virtual int GetID() override { return _id; }

private:
  Mat1Data _mat1_data;
};
} // namespace COMPONENT