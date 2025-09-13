#include "factory_psolid.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> PsolidFactory::CreateComp() {
  return boost::make_shared<COMPONENT::PSOLID>();
}
}  // namespace FACTORY