#pragma once
#include "component/element/D2/CQUAD4_mindlin.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class Cquad4MlFactory : public FactoryBase {
public:
  Cquad4MlFactory() = default;
  ~Cquad4MlFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY