#pragma once
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>
namespace TOOL {
// 1～3重积分在标准空间下(积分变量属于[-1,
// 1])的高斯积分计算函数(1～4个高斯积分点)
boost::shared_ptr<double> GaussIntegral(const std::string& func,
                                        const int& num_r,
                                        const int& num_integal);

}  // namespace TOOL