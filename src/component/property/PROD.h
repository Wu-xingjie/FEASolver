#pragma once

#include "material/mat1.h"
#include "property_base.h"

namespace COMPONENT {
// 打包杆单元属性参数
struct ProdData {
  ProdData() : _a(-1.0), _mat(), _l(-1.0) {}
  ProdData(const ProdData& p);
  double _a;  // 截面面积
  MAT1 _mat;
  double _l;  // 单元长度
};

class PROD : public PropertyBase {
 public:
  PROD() : PropertyBase() {}
  ~PROD() = default;
  PROD(const PROD& p);

  // 设置对应属性参数
  void SetProperty(double a, MAT1 mat, double l);

  // 返回对应属性参数
  ProdData GetProperty();

 private:
  ProdData _prod_data;
};
}  // namespace COMPONENT