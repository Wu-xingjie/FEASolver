#pragma once
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <eigen3/Eigen/Dense>
namespace TOOL {
// 计算物理域为正三棱柱空间的高斯积分
boost::shared_ptr<double> TriGaussIntegral(
    const std::string& func,
    const std::vector<Eigen::Vector2d>& points,
    const int& num_intergral_point, const double& t);
}  // namespace TOOL