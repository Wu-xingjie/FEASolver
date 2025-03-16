#pragma once

#include <array>
#include <string>

#include "coordinate/cartesian_coordinate.h"

namespace COMPONENT {
typedef Eigen::Vector3d vec_3;
// 定义节点类型
class Node {
 public:
  Node() = default;
  Node(const double &x, const double &y, const double &z,
       const CartesianCoord &c)
      : _x(x), _y(y), _z(z), _coord(c) {}
  ~Node() = default;
  // 拷贝构造
  Node(const Node &n);

  // 获取节点坐标
  vec_3 get_location() const;
  // 获取节点坐标系
  CartesianCoord GetCoord() const;
  // 设置节点坐标
  void set_node(const double &x, const double &y, const double &z,
                const CartesianCoord &coord);

 private:
  CartesianCoord _coord;
  double _x;
  double _y;
  double _z;
};
}  // namespace COMPONENT
