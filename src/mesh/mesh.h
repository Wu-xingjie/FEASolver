#pragma once

#include <boost/shared_ptr.hpp>
#include <set>

namespace MESH {
// 设计一个网格类，负责作为容器管理整个有限元模型的网格信息
class Mesh {
public:
  Mesh() = default;
  ~Mesh() = default;

    

private:
  boost::shared_ptr<std::set<MESH::Node>> _node;        // 节点库
  boost::shared_ptr<std::set<MESH::ElemBase>> _element; // 单元库
};
} // namespace MESH