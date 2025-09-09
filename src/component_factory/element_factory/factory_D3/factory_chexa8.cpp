#include "factory_chexa8.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> Chexa8Factory::CreateComp() {
  return boost::make_shared<COMPONENT::Chexa8>();
}
}  // namespace FACTORY