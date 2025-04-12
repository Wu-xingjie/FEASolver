#pragma once

#include "component/property/PROD.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class ProdFactory : public FactoryBase {
public:
  ProdFactory() = default;
  ~ProdFactory() = default;
  boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY