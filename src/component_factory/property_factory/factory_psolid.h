#pragma once

#include "component/property/PSOLID.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class PsolidFactory : public FactoryBase {
public:
  PsolidFactory() = default;
  ~PsolidFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY