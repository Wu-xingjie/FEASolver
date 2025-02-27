#pragma once

#include <array>
#include <string>

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
  double _x;
  double _y;
  double _z;
  // TODO: 等坐标系设计好之后回来设计节点坐标系
};
}  // namespace NODE
