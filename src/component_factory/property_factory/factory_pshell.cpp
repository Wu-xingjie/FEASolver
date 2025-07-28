#include "factory_pshell.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> PshellFactory::CreateComp() {
  return boost::make_shared<COMPONENT::PSHELL>();
}
}  // namespace FACTORY