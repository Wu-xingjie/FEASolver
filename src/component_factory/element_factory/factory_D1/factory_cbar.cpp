#include "factory_cbar.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> CBARFactory::CreateComp() {
  return boost::make_shared<COMPONENT::BAR>();
}
}  // namespace FACTORY