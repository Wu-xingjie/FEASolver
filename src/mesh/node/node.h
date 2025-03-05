#pragma once

#include <array>
#include <string>

#include "coordinate/cartesian_coordinate.h"

namespace NODE {
// 定义节点类型
class Node {
 public:
  Node() = default;
  ~Node() = default;

  // 获取节点坐标
  std::array<double, 3> get_location();
  // 在制定节点坐标
  void set_location(const double& x, const double& y, const double& z,
                    const std::string& coord = "global");

 private:
  COORDINATE::CartesianCoord _coordinate;
  double _x;
  double _y;
  double _z;
};
}  // namespace NODE
