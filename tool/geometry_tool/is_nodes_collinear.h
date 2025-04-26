#pragma once
#include <boost/shared_ptr.hpp>
#include <vector>
#include <eigen3/Eigen/Dense>
namespace TOOL {
// 判断同一坐标系下的多个点是否共线
bool IsNodesCollinear(std::vector<Eigen::Vector3d> points);
}  // namespace TOOL