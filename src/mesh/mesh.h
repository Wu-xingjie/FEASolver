#pragma once

#include <boost/shared_ptr.hpp>
#include <set>
#include <string>

namespace MESH {
// 设计一个网格类，负责作为容器管理整个有限元模型的网格信息
class Mesh {
 public:
  Mesh() = default;
  ~Mesh() = default;

  // TODO: 等节点、单元定义好之后回来写节点和单元的接口

 private:
  boost::shared_ptr<std::set<MESH::Node>> _node;         // 节点库
  boost::shared_ptr<std::set<MESH::ElemBase>> _element;  // 单元库
};
}  // namespace MESH