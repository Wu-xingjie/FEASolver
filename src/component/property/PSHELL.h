#pragma once
#include "property_base.h"

namespace COMPONENT {
class PSHELL : public PropertyBase {
 public:
  PSHELL() = default;
  ~PSHELL() = default;

  // 设置属性参数
  virtual void SetComp(const file_data &datas) override;

  virtual std::map<std::string, boost::any> GetPropDate() override;

 private:
  int _mid1;
  double _t;
  double _mid2;
  double _inertia_ratio_of_bm{1.0};
};
}  // namespace COMPONENT