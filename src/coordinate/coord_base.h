#pragma once

#include <array>

namespace COORDINATE {
class CoordBase {
 public:
  CoordBase(const double& factor1, const double& factor2, const double& factor3)
      : _factor1(factor1), _factor2(factor2), _factor3(factor3) {}
  ~CoordBase() = default;

  // 获取坐标系id
  double getID();
  // 获取坐标系参数
  std::array<double, 3> getFactor();

 private:
  double _coord_id;  // TODO:id需要利用id_manager进行管理
  // 全局坐标系默认为原点为(0,0,0)、xyz轴分别(1,0,)、(0,1,0)、(0,0,1)的坐标系
  double _factor1;
  double _factor2;
  double _factor3;
};
}  // namespace COORDINATE