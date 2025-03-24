#pragma once

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "component/element/elemen_base.h"
#include "component/node/node.h"

namespace MODEL {
// 设计一个网格类，负责作为容器管理整个有限元模型的网格信息
class Model {
public:
  Model() = default;
  ~Model() = default;

private:
  std::vector<boost::shared_ptr<COMPONENT::Node>> _node;        // 节点库
  std::vector<boost::shared_ptr<COMPONENT::ElemBase>> _element; // 单元库
};
} // namespace MODEL