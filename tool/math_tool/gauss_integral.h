#pragma once
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>
namespace TOOL {
// 1～3重积分的高斯积分计算函数
boost::shared_ptr<double> GaussIntegral(const std::string& func,
                                        const double& idx_up,
                                        const double& idx_low,
                                        const int& num_r,
                                        const int& num_integal);

}  // namespace TOOL