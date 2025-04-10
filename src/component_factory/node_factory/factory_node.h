#pragma once

#include "component/node/node.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class NodeFactory : public FactoryBase {
public:
  NodeFactory() = default;
  ~NodeFactory() = default;
  boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY