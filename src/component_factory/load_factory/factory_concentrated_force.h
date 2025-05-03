#pragma once

#include "component/load/concentrated_force.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class ConcentratedForceFactory : public FactoryBase {
public:
  ConcentratedForceFactory() = default;
  ~ConcentratedForceFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY