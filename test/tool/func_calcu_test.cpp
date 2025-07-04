#include "math_tool/func_expr/func_cal.h"
#include <iostream>
#include <string>
int main() {
  std::string func = "4*x^3*(5*y^2+6*y*(2*x+3*x))+7*(8.3+3*x^2*(3+x))+4.3+x^5";
  double val = TOOL::FuncCal(func, 1.0, 1.0);
  std::cout << "value = " << val << std::endl;
  return 0;
}