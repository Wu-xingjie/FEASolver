#include "factory_cquad4_ml.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> Cquad4MlFactory::CreateComp() {
  return boost::make_shared<COMPONENT::Cquad4Mindlin>();
}
}  // namespace FACTORY