#include <iostream>
#include <vector>

#include "math_tool/tri_gauss_integral.h"
int main() {
  std::string func = "(x+1)";
  Eigen::Vector2d p1 = {0.0, 0.0};
  Eigen::Vector2d p2 = {4.0, 0.0};
  Eigen::Vector2d p3 = {0.0, 2.0};
  std::vector<Eigen::Vector2d> points{p1, p2, p3};
  auto result = TOOL::TriGaussIntegral(func, points, 3, 0.5);
  if (result) {
    std::cout << "result = " << *result << std::endl;
  } else {
    std::cout << "result is none" << std::endl;
  }
  return 0;
}