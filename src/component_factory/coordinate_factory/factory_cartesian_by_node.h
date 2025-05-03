#pragma once

#include "component/coordinate/cartesian_by_node.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class CartesianByNodeFactory : public FactoryBase {
public:
  CartesianByNodeFactory() = default;
  ~CartesianByNodeFactory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY