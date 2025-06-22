#pragma once
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>
namespace TOOL {

std::vector<std::string> GaussIntegral(const int &num_r,
                                       const std::vector<std::string>& func);

} // namespace TOOL