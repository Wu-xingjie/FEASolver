#include "math_tool/area_func_expr.h"
#include <iostream>
int main() {
  Eigen::Vector2d p_1{1.2, 3.4};
  Eigen::Vector2d p_2{3.6, 9.5};
  std::cout << "result:" << std::endl
            << TOOL::AreaFuncExpr(p_1, p_2) << std::endl;
  return 0;
}