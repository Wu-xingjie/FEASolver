#pragma once
#include "component/element/D2/CQUAD4_kirchhoff.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class Cquad4KfFactory : public FactoryBase {
public:
  Cquad4KfFactory() = default;
  ~Cquad4KfFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY