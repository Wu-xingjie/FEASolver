#include "parser.h"

#include <iostream>

int main() {
  std::string addr =
      "/home/wxj/workspace/FEASolver/file/test_file/parser_test.bdf";
  MAPPER::Parser parser;
  std::cout << "file address: " << addr << std::endl;
  parser.ParserFile(addr);
  std::vector<std::array<boost::any, 10>> data = parser.GetData();
  for (auto elem : data) {
    for (int i = 0; i < elem.size(); i++) {
      if (!elem.at(i).empty()){
        if (elem.at(i).type() == typeid(std::string)) {
          std::cout << boost::any_cast<std::string>(elem.at(i)) << " ";
        } else {
          std::cout << boost::any_cast<double>(elem.at(i)) << " ";
        }
    }
  }
  std::cout << std::endl;
}
return 0;
}