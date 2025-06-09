#include "factory_concentrated_moment.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> ConcentratedMomentFactory::CreateComp() {
  return boost::make_shared<COMPONENT::ConcentratedMoment>();
}
} // namespace FACTORY