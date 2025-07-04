#include <iostream>

#include "math_tool/gauss_integral.h"
int main() {
  std::string func = "(x+1)*(y+4)^2";
  auto result = TOOL::GaussIntegral(func, 2, 2);
  std::cout << "函数(x+1)*(y+4)^2在自然区间内的积分: " << *result << std::endl;

  std::string func1 = "cos(x)*2.718^y*cos(z)";
  auto result1 = TOOL::GaussIntegral(func1, 4, 3);
  std::cout << "函数cos(x)*2.718^y*cos(z)在自然区间内的积分: " << *result1 << std::endl;

  return 0;
}