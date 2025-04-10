#include "factory_node.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> NodeFactory::CreateComp() {
  return boost::make_shared<COMPONENT::Node>();
}
}  // namespace FACTORY