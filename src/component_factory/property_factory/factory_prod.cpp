#include "factory_prod.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> ProdFactory::CreateComp() {
  return boost::make_shared<COMPONENT::PROD>();
}
}  // namespace FACTORY