#pragma once
#include <array>
#include "id_manager/id_mananger.h"
#include <eigen3/Eigen/Dense>
#include "model_mapper/register.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace COMPONENT {
typedef Eigen::Vector3d vec_3;
class CoordBase {
public:
  CoordBase() { _coord_id = TOOL::IdManager(TOOL::id_type::COORDINATE); }

  CoordBase(const vec_3 &origin, const vec_3 &vec1, const vec_3 &vec2,
            const vec_3 &vec3, const bool &assign_id = true)
      : _coord_origin(origin), _vec1(vec1), _vec2(vec2), _vec3(vec3) {
    if (assign_id) {
      _coord_id = TOOL::IdManager(TOOL::id_type::COORDINATE);
    }
  }

  ~CoordBase() = default;

  // (接口)获取坐标系id
  virtual int getID() = 0;
  // (接口)获取坐标系原点
  virtual vec_3 getORIGIN() = 0;
  // (接口)获取坐标轴
  virtual std::array<vec_3, 3> getVEC() = 0;

  // 判断坐标系是否为相等
  bool operator==(const CoordBase &c) {
    return (_coord_id == c._coord_id) ? true : false;
  }

protected:
  int _coord_id;
  // 全局坐标系默认为原点为(0,0,0)、xyz轴分别(1,0,0)、(0,1,0)、(0,0,1)的坐标系
  // 坐标原点
  Eigen::Vector3d _coord_origin;
  // 坐标轴
  Eigen::Vector3d _vec1;
  Eigen::Vector3d _vec2;
  Eigen::Vector3d _vec3;
};
} // namespace COMPONENT