#include "factory_tria3.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> TRIA3Factory::CreateComp() {
  return boost::make_shared<COMPONENT::Tri3>();
}
}  // namespace FACTORY