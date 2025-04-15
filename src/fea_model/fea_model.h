#pragma once

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "component/component_base.h"
// #include "component/coordinate/coordinate_base.h"
// #include "component/element/elemen_base.h"
// #include "component/load/load_base.h"
// #include "component/material/material_base.h"
// #include "component/node/node.h"
// #include "component/property/property_base.h"

namespace MODEL {
// 设计一个网格类，负责作为容器管理整个有限元模型的网格信息
class Model {
 public:
  Model() = default;
  ~Model() = default;

  void InsertComp(const boost::shared_ptr<COMPONENT::CompBase> p);

 
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _node;         // 节点库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _element;  // 单元库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _coord;   // 坐标系库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _load;     // 载荷库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _prop;  // 属性库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _mat;   // 材料库
};
}  // namespace MODEL