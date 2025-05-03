#pragma once
#include "component/component_base.h"
namespace COMPONENT {
class ConstrainBase : public CompBase {
public:
  ConstrainBase() { _type = CompBase::comp_type::constrain; }
  virtual ~ConstrainBase() = default;
};
} // namespace COMPONENT