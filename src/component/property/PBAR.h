#pragma once
#include "property_base.h"

namespace COMPONENT {
// 打包杆单元属性参数
// struct ProdData {
//   ProdData() = default;
//   ProdData(const ProdData &p);
//   double _a; // 截面面积
//   int _mid;
// };

class PBAR : public PropertyBase {
 public:
  PBAR() = default;
  ~PBAR() = default;

  // 设置属性参数
  void SetComp(const file_data &datas);

  virtual double GetCrossArea() override { return _a; }

  virtual int GetMatId() override { return _mid; }

 private:
  int _mid;
  double _a;
  double _I1;
  double _I2;
  double _J;
};
}  // namespace COMPONENT