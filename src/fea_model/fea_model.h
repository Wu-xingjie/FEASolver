#pragma once

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "component/coordinate/coordinate_base.h"
#include "component/element/elemen_base.h"
#include "component/load/load_base.h"
#include "component/material/material_base.h"
#include "component/node/node.h"
#include "component/property/property_base.h"

namespace MODEL {
// 设计一个网格类，负责作为容器管理整个有限元模型的网格信息
class Model {
 public:
  Model() = default;
  ~Model() = default;

 private:
  std::vector<boost::shared_ptr<COMPONENT::Node>> _node;         // 节点库
  std::vector<boost::shared_ptr<COMPONENT::ElemBase>> _element;  // 单元库
  std::vector<boost::shared_ptr<COMPONENT::CoordBase>> _coord;   // 坐标系库
  std::vector<boost::shared_ptr<COMPONENT::LoadBase>> _load;     // 载荷库
  std::vector<boost::shared_ptr<COMPONENT::PropertyBase>> _prop; // 属性库
  std::vector<boost::shared_ptr<COMPONENT::MaterialBase>> _mat;  // 材料库
};
}  // namespace MODEL