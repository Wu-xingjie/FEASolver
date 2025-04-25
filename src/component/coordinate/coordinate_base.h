#pragma once
#include <array>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

#include "component/component_base.h"
#include "model_mapper/register.h"

namespace COMPONENT {
typedef Eigen::Vector3d vec_3;
// 一般坐标系,全局坐标系默认id为0,原点为(0,0,0),xyz轴分别(1,0,0)、(0,1,0)、(0,0,1)
struct GeneralCoord {
  GeneralCoord() = default;
  // 拷贝构造
  GeneralCoord(const GeneralCoord &p) {
    _coord_origin = p._coord_origin;
    _vec1 = p._vec1;
    _vec2 = p._vec2;
    _vec3 = p._vec3;
    _coord_flag = p._coord_flag;
  }
  // 坐标原点
  vec_3 _coord_origin;
  // 标记一般坐标系是否被创建
  bool _coord_flag{false};
  // 坐标轴
  vec_3 _vec1;
  vec_3 _vec2;
  vec_3 _vec3;
};

// 坐标系基类
class CoordBase : public CompBase {
 public:
  CoordBase() { _type = comp_type::coord; };
  virtual ~CoordBase() = default;

  // (接口)获取坐标系id
  virtual int GetID() { return _id; }
  // (接口)获取坐标系原点
  virtual vec_3 getORIGIN() { return _general_coord._coord_origin; };
  // (接口)获取坐标轴
  virtual std::array<vec_3, 3> getVEC() {
    std::array<vec_3, 3> axi_vec = {_general_coord._vec1, _general_coord._vec2,
                                    _general_coord._vec3};
    return axi_vec;
  }
  // 判断坐标系是否为相等
  bool operator==(const CoordBase &c) { return (_id == c._id) ? true : false; }
  // 给元件赋值
  virtual void SetComp(const file_data &datas) = 0;
  GeneralCoord GetGeneralCoord() { return _general_coord; }
  // 创建易于求解器计算的坐标系（原点坐标+三个vector构成的坐标轴）
  virtual void TransToGeneralCoord() = 0;

 protected:
  int _id;
  GeneralCoord _general_coord;
};
}  // namespace COMPONENT