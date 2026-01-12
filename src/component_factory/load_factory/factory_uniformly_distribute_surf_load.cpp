#include "factory_uniformly_distribute_surf_load.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> UnifDistributeSurfLoadFactory::CreateComp() {
  return boost::make_shared<COMPONENT::UniDistributeSurfLoad>();
}
} // namespace FACTORY