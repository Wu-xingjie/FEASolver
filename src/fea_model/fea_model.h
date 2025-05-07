#pragma once
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "component/component_base.h"

namespace MODEL {
// 存储整个模型的信息
class Model {
public:
  Model();
  ~Model() = default;

  void InsertComp(const boost::shared_ptr<COMPONENT::CompBase> p);

  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _node;    // 节点库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _element; // 单元库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _coord;   // 坐标系库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _load;    // 载荷库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _prop;    // 属性库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _mat;     // 材料库
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> _constrain; // 约束库
};
} // namespace MODEL