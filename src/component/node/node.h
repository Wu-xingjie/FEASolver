#pragma once

#include <array>
#include <string>
#include "id_manager/id_mananger.h"
#include <eigen3/Eigen/Dense>
#include "model_mapper/register.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace COMPONENT {
typedef Eigen::Vector3d vec_3;
// 定义节点类型
class Node {
 public:
  Node() = default;
  Node(const double &x, const double &y, const double &z,
       const int &c)
      : _x(x), _y(y), _z(z), _coord(c) {}
  ~Node() = default;
  // 拷贝构造
  Node(const Node &n);

  // 获取节点坐标
  vec_3 get_location() const;
  // 获取节点坐标系
  int GetCoord() const;
  // 设置节点坐标
  void set_node(const double &x, const double &y, const double &z,
                const int &coord);

 private:
  int _coord;
  double _x;
  double _y;
  double _z;
};
}  // namespace COMPONENT
