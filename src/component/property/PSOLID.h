#pragma once
#include "property_base.h"

namespace COMPONENT {
class PSOLID : public PropertyBase {
 public:
  PSOLID() = default;
  ~PSOLID() = default;

  // 设置属性参数
  virtual void SetComp(const file_data &datas) override;

  virtual std::map<std::string, boost::any> GetPropDate() override;

 private:
  int _mid;
};
}  // namespace COMPONENT