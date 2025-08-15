#include "factory_cquad4_kf.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> Cquad4KfFactory::CreateComp() {
  return boost::make_shared<COMPONENT::Cquad4Kf>();
}
}  // namespace FACTORY