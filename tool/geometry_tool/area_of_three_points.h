#pragma once
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

namespace TOOL {
// 计算平面内三个点所围成三角形区域的面积
boost::shared_ptr<double> AreaOfThreePoints(const Eigen::Vector2d& p1,
                                            const Eigen::Vector2d& p2,
                                            const Eigen::Vector2d& p3);
}  // namespace TOOL