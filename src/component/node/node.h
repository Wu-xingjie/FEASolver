#pragma once

#include <array>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <string>

#include "component/component_base.h"
#include "id_manager/id_mananger.h"
#include "model_mapper/register.h"

namespace COMPONENT {
typedef Eigen::Vector3d vec_3;
// 定义节点类型
class Node : public CompBase {
 public:
  Node() { _type = comp_type::node; };
  Node(const double &x, const double &y, const double &z, const int &c)
      : _x(x), _y(y), _z(z), _coord(c) {}
  ~Node() = default;
  // 拷贝构造
  Node(const Node &n);

  int GetID() override { return _id; }
  // 设置节点坐标
  void SetComp(const file_data &datas) override;
  comp_type Type() override { return _type; }
  // 获取节点坐标
  vec_3 get_location() const;
  // 获取节点坐标系
  int GetCoord() const;

 private:
  int _id;
  int _coord;
  double _x;
  double _y;
  double _z;
};
}  // namespace COMPONENT
