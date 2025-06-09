#pragma once

#include "component/load/concentrated_moment.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class ConcentratedMomentFactory : public FactoryBase {
public:
  ConcentratedMomentFactory() = default;
  ~ConcentratedMomentFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY