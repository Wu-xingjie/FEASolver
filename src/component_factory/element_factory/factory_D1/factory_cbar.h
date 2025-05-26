#pragma once
#include "component/element/D1/BAR.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class CBARFactory : public FactoryBase {
public:
  CBARFactory() = default;
  ~CBARFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY