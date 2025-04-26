#pragma once
#include "component/coordinate/coordinate_base.h"
namespace TOOL {
// 获取坐标系obj到坐标系base的坐标变换矩阵
Eigen::Matrix3d
TransCoordToCoord(const boost::shared_ptr<COMPONENT::CoordBase> base,
                  const boost::shared_ptr<COMPONENT::CoordBase> obj);
} // namespace TOOL