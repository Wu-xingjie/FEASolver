#pragma once

#include <array>

#include "id_manager/id_mananger.h"

namespace COORDINATE {
typedef std::array<double, 3> arr3;
class CoordBase {
 public:
  CoordBase(const arr3& origin, const arr3& vec1, const arr3& vec2,
            const arr3& vec3)
      : _coord_origin(origin), _vec1(vec1), _vec2(vec2), _vec3(vec3) {
    _coord_id = TOOL::IdManager(TOOL::id_type::COORDINATE);
  }
  ~CoordBase() = default;

  // (接口)获取坐标系id
  virtual int getID() = 0;
  // (接口)获取坐标系原点
  virtual arr3 getORIGIN() = 0;
  // (接口)获取坐标轴
  virtual std::array<arr3, 3> getVEC() = 0;

 protected:
  double _coord_id;
  // 全局坐标系默认为原点为(0,0,0)、xyz轴分别(1,0,)、(0,1,0)、(0,0,1)的坐标系
  // 坐标原点
  arr3 _coord_origin;
  // 坐标轴
  arr3 _vec1;
  arr3 _vec2;
  arr3 _vec3;
};
}  // namespace COORDINATE