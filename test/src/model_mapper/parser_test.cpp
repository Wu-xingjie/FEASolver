#include "parser.h"

#include <iostream>

int main() {
  std::string addr =
      "/home/wxj/workspace/FEASolver/file/test_file/parser_test.bdf";
  MAPPER::Parser parser;
  std::cout << "file address: " << addr << std::endl;
  parser.ParserFile(addr);
  std::vector<std::array<boost::any, 20>> data = parser.GetData();
  for (auto elem : data) {
    for (int i = 0; i < elem.size(); i++) {
      if (!elem.at(i).empty()) {
        if (elem.at(i).type() == typeid(std::string)) {
          std::cout << boost::any_cast<std::string>(elem.at(i)) << " ";
        } else if (elem.at(i).type() == typeid(double)) {
          std::cout << boost::any_cast<double>(elem.at(i)) << " ";
        }else{
          std::cout << boost::any_cast<int>(elem.at(i)) << " ";
        }
      }
    }
    std::cout << std::endl;
  }

  parser.DividedByComp();
  auto comp_datas = parser.GetCompsData();
  int comp_num = 1;
  for (auto comp : comp_datas) {
    std::cout << comp_num << "th comp:" << std::endl;
    comp_num += 1;
    for (auto elem : comp) {
      for (int i = 0; i < elem.size(); i++) {
        if (!elem.at(i).empty()) {
          if (elem.at(i).type() == typeid(std::string)) {
            std::cout << boost::any_cast<std::string>(elem.at(i)) << " ";
          } else if (elem.at(i).type() == typeid(double)){
            std::cout << boost::any_cast<double>(elem.at(i)) << " ";
          }else{
            std::cout << boost::any_cast<int>(elem.at(i)) << " ";
          }
        }
      }
      std::cout << std::endl;
    }
  }

  return 0;
}