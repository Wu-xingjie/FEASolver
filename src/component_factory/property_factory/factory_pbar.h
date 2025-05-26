#pragma once

#include "component/property/PBAR.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class PbarFactory : public FactoryBase {
public:
  PbarFactory() = default;
  ~PbarFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY