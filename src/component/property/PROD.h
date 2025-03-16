#pragma once

#include "component/material/mat1.h"
#include "property_base.h"

namespace COMPONENT {
// 打包杆单元属性参数
struct ProdData {
  ProdData() : _a(-1.0), _mat() {}
  ProdData(const ProdData& p);
  double _a;  // 截面面积
  MAT1 _mat;
};

class PROD : public PropertyBase {
 public:
  PROD() : PropertyBase() {}
  ~PROD() = default;
  PROD(const PROD& p);

  // 设置对应属性参数
  void SetProperty(double a, MAT1 mat);

  // 返回对应属性参数
  ProdData GetProperty();

 private:
  ProdData _prod_data;
};
}  // namespace COMPONENT