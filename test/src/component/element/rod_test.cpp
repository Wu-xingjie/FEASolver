#include "component/element/D1/ROD.h"
#include "mapper.h"
#include <iostream>
int main() {
  // 输入文件地址
  std::string inpfile_addr = "/home/wxj/workspace/FEASolver/file/test_file/parser_test.bdf";
  // 解析文件
  MAPPER::Parser parser;
  parser.ParserFile(inpfile_addr);
  parser.DividedByComp();
  auto comp_datas = parser.GetCompsData();
  // 创建文件模型
  MODEL::Model model;
  // 文件映射到文件模型
  MAPPER::FileToMapper mapper;
  mapper.mapper(model, comp_datas);
  // 生成单元刚度矩阵
  auto &base_elem = model._element.front();
  auto elem = boost::dynamic_pointer_cast<COMPONENT::ROD>(base_elem);
  elem->GenerateK(model);
  // 获取单元刚度矩阵
  std::cout << "单元刚度矩阵为:" << std::endl;
  std::cout << elem->GetK() << std::endl;

  return 0;
}