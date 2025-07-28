#pragma once

#include "component/property/PSHELL.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class PshellFactory : public FactoryBase {
public:
  PshellFactory() = default;
  ~PshellFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY