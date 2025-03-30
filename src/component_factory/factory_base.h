#pragma once
#include "component/component_base.h"
namespace FACTORY {
class FactoryBase {
public:
  FactoryBase() = default;
  virtual ~FactoryBase() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() = 0;
};
} // namespace FACTORY