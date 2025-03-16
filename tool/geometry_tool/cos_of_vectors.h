#include <eigen3/Eigen/Dense>
#include <iostream>
#include <stdexcept>

namespace TOOL {
  //求两个三维向量夹角的cos值，返回的是一个弧度
double CosOfVecs(const Eigen::Vector3d &v1, const Eigen::Vector3d &v2);
} // namespace TOOL