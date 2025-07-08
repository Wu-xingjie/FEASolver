#include "is_nodes_collinear.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "cos_of_vectors.h"
#define PI 3.14159
namespace TOOL {
bool IsNodesCollinear(std::vector<Eigen::Vector3d> points) {
  bool result = true;
  if (points.size() < 3) {
    throw std::runtime_error(
        "[ERROR]:func(IsNodesCollinear)>>>传入节点数必须大于2");
  }
  // auto vec1 = points.at(1) - points.at(0);
  for (int i = 0; i < points.size() - 2; i++) {
    auto vec_front = points.at(i + 2) - points.at(i);
    auto vec_back = points.at(i + 1) - points.at(i);
    //   计算夹角
    auto cos_theta = CosOfVecs(vec_back, vec_front);
    // if ((cos_theta > 1.0e-11) || ((cos_theta - 3.1415926) > 1.0e-11)) {
    if (std::abs(std::abs(cos_theta) - 1.0) > 1e-11) {
      result = false;
    }
  }
  return result;
}
}  // namespace TOOL