#include "is_nodes_collinear.h"

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
    auto vec_front = points.at(i + 2) - points.at(i + 1);
    auto vec_back = points.at(i + 1) - points.at(i + 2);
    //   计算夹角
    auto theta = CosOfVecs(vec_back, vec_front);
    if ((theta > 1.0e-11) || ((theta - PI) > 1.0e-11)) {
      result = false;
    }
  }
  return result;
}
}  // namespace TOOL