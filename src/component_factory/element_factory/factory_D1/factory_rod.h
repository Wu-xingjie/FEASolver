#pragma once
#include "component/element/D1/ROD.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class RodFactory : public FactoryBase {
public:
  RodFactory() = default;
  ~RodFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY