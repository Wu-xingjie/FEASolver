#include "get_optional_value/get_optional_value.h"

#include <iostream>
#include <string>
int main() {
  boost::optional<int> pi = 23;
  boost::optional<std::string> ps("wxj");
  boost::optional<double> pd = 23.12;
  boost::optional<double> pundefine;

  int i = -1;
  std::string s = "n";
  double d = 0.1;
  double undefine = -0.1;



  std::cout << "i = " << i << std::endl;
  std::cout << "s = " << s << std::endl;
  std::cout << "d = " << d << std::endl;
  std::cout << undefine << std::endl;
  return 0;
}