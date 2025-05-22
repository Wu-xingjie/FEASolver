#pragma once
#include "property_base.h"

namespace COMPONENT {
class PBAR : public PropertyBase {
 public:
  PBAR() = default;
  ~PBAR() = default;

  // 设置属性参数
  virtual void SetComp(const file_data &datas) override;

  virtual std::map<std::string, boost::any> GetPropDate() override;

 private:
  int _mid;
  double _a;
  double _I1;
  double _I2;
  double _J;
};
}  // namespace COMPONENT