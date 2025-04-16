#pragma once
#include "property_base.h"

namespace COMPONENT {
// 打包杆单元属性参数
struct ProdData {
  ProdData() = default;
  ProdData(const ProdData &p);
  double _a; // 截面面积
  int _mid;
};

class PROD : public PropertyBase {
public:
  PROD() = default;
  ~PROD() = default;
  PROD(const PROD &p);

  // 设置属性参数
  void SetComp(const file_data &datas);

  // 返回对应属性参数
  virtual ProdData GetProperty() override;

private:
  ProdData _prod_data;
};
} // namespace COMPONENT