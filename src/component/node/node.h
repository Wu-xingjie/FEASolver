#pragma once

#include <array>
#include <string>

#include "coordinate/cartesian_coordinate.h"

namespace NODE {
typedef std::array<double, 3> arr3;
// 定义节点类型
class Node {
public:
  Node() = default;
  Node(const double &x, const double &y, const double &z, const int &c)
      : _x(x), _y(y), _z(z), _cid(c) {}
  ~Node() = default;
  // 拷贝构造
  Node(const Node &n);

  // 获取节点坐标
  arr3 get_location();
  // 设置节点坐标
  void set_node(const double &x, const double &y, const double &z,
                const std::string &coord = "global");

private:
  int _cid;
  double _x;
  double _y;
  double _z;
};
} // namespace NODE
