#include "factory_rod.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> RodFactory::CreateComp() {
  return boost::make_shared<COMPONENT::ROD>();
}
}  // namespace FACTORY