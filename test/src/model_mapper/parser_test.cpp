#include "parser.h"
#include <iostream>

int main() {
  std::string addr =
      "/home/wxj/workspace/FEASolver/file/test_file/parser_test.bdf";
  MAPPER::Parser parser;
  std::cout << "file address: " << addr << std::endl;
  parser.ParserFile(addr);
  std::vector<std::vector<boost::any>> data = parser.GetData();
  for (auto elem : data) {
    for (auto i : elem) {
      if (i.type() == typeid(std::string)) {
        std::cout << boost::any_cast<std::string>(i) << " ";
      } else {
        std::cout << boost::any_cast<double>(i) << " ";
      }
    }
    std::cout << std::endl;
  }
  return 0;
}