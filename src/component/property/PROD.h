#pragma once
#include "property_base.h"
#include <boost/any.hpp>

namespace COMPONENT {
class PROD : public PropertyBase {
 public:
  PROD() = default;
  ~PROD() = default;
  PROD(const PROD &p);

  // 设置属性参数
  void SetComp(const file_data &datas);

  virtual std::map<std::string, boost::any> GetPropDate() override;

 private:
  double _a;  // 截面面积
  int _mid;   // 材料id
};
}  // namespace COMPONENT