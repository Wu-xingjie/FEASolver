#pragma once

#include "component/constrained/single_point_constrained.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class SinglePointConstrainFactory : public FactoryBase {
public:
  SinglePointConstrainFactory() = default;
  ~SinglePointConstrainFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY