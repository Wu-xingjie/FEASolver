#include "factory_pbar.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> PbarFactory::CreateComp() {
  return boost::make_shared<COMPONENT::PBAR>();
}
}  // namespace FACTORY