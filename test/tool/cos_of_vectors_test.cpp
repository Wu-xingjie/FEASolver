#include "geometry_tool/cos_of_vectors.h"
#include <cmath>
#include <iostream>

int main() {
  Eigen::Vector3d vec1(1, 0, 0);
  Eigen::Vector3d test(std::cos(M_PI * 30 / 180), std::sin(M_PI * 30 / 180), 0);
  std::cout << "cos is " << TOOL::CosOfVecs(vec1, test) << std::endl;
  return 0;
}
