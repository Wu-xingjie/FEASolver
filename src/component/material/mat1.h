#pragma once

#include <boost/optional.hpp>

#include "material_base.h"

namespace COMPONENT {
// 各项同性材料
class MAT1 : public MaterialBase {
 public:
  MAT1() = default;
  ~MAT1() = default;
  MAT1(const MAT1 &p);

  virtual void SetComp(const file_data &datas) override;

  virtual std::map<std::string, boost::any> GetMatDate() override;

 private:
  boost::optional<double> _E;
  boost::optional<double> _NU;
  boost::optional<double> _G;
};
}  // namespace COMPONENT