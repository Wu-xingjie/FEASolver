#include "factory_cartesian_by_node.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> CartesianByNodeFactory::CreateComp() {
  return boost::make_shared<COMPONENT::CartesianCoord>();
}
} // namespace FACTORY