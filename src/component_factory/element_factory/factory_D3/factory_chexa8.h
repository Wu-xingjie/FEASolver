#pragma once
#include "component/element/D3/CHEXA8.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class Chexa8Factory : public FactoryBase {
public:
  Chexa8Factory() = default;
  ~Chexa8Factory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY