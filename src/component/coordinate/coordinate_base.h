#pragma once
#include "component/component_base.h"
#include "model_mapper/register.h"
#include <array>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

namespace COMPONENT {
typedef Eigen::Vector3d vec_3;
// 一般坐标系
struct GeneralCoord {
  // 全局坐标系默认为原点为(0,0,0)、xyz轴分别(1,0,0)、(0,1,0)、(0,0,1)的坐标系
  // 坐标原点
  Eigen::Vector3d _coord_origin;
  // 坐标轴
  vec_3 _vec1;
  vec_3 _vec2;
  vec_3 _vec3;
};

class CoordBase : public CompBase {
public:
  CoordBase() { _type = comp_type::coord; };
  ~CoordBase() = default;

  // (接口)获取坐标系id
  virtual int GetID() = 0;
  // (接口)获取坐标系原点
  virtual vec_3 getORIGIN() = 0;
  // (接口)获取坐标轴
  virtual std::array<vec_3, 3> getVEC() = 0;
  // 判断坐标系是否为相等
  bool operator==(const CoordBase &c) { return (_id == c._id) ? true : false; }
  // 给元件赋值
  virtual void SetComp(const file_data &datas) = 0;
  virtual comp_type Type() = 0;

  // 创建易于求解器计算的坐标系（原点坐标+三个vector构成的坐标轴）
  virtual GeneralCoord GetGeneralCoord() = 0;

protected:
  int _id;
  GeneralCoord _general_coord;
};
} // namespace COMPONENT