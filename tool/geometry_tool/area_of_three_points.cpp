#include "area_of_three_points.h"

#include <boost/make_shared.hpp>

#include "is_nodes_collinear.h"

namespace TOOL {
boost::shared_ptr<double> AreaOfThreePoints(const Eigen::Vector2d& p1,
                                            const Eigen::Vector2d& p2,
                                            const Eigen::Vector2d& p3) {
  // 判断三个点是否共线
  Eigen::Vector3d temp_p1{p1(0), p1(1), 0.0};
  Eigen::Vector3d temp_p2{p2(0), p2(1), 0.0};
  Eigen::Vector3d temp_p3{p3(0), p3(1), 0.0};
  if (IsNodesCollinear({temp_p1, temp_p2, temp_p3})) {
    throw std::runtime_error(
        "[ERROR]:func(AreaOfThreePoints)>>>"
        "传入的三个点共线，无法计算三角形面积");
  }
  // 计算三角形区域面积
  auto result = boost::make_shared<double>();
  *result = 0.5 * (p2(0) * p3(1) + p1(0) * p2(1) + p1(1) * p3(0) -
                   p2(0) * p1(1) - p1(0) * p3(1) - p2(1) * p3(0));
  return result;
}
}  // namespace TOOL