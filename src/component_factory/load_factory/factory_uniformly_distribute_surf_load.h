#pragma once

#include "component/load/distributed_load/uniformly_distribute_surf_load.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class UnifDistributeSurfLoadFactory : public FactoryBase {
public:
  UnifDistributeSurfLoadFactory() = default;
  ~UnifDistributeSurfLoadFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY