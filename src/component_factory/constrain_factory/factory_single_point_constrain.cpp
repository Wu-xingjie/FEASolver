#include "factory_single_point_constrain.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase>
SinglePointConstrainFactory::CreateComp() {
  return boost::make_shared<COMPONENT::SinglePointConstrains>();
}
} // namespace FACTORY