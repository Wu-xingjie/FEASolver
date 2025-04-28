#pragma once
#include "component/component_base.h"

namespace COMPONENT {
class LoadBase : public CompBase {
 public:
  LoadBase() { _type = comp_type::load; }
  virtual ~LoadBase() = default;

  virtual void SetComp(const file_data &datas) = 0;
};
}  // namespace COMPONENT