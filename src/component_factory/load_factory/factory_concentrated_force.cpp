#include "factory_force.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> ConcentratedForceFactory::CreateComp() {
  return boost::make_shared<COMPONENT::ConcentratedForce>();
}
} // namespace FACTORY