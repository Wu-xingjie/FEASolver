#include "mapper.h"

int main() {
  std::string addr =
      "/home/wxj/workspace/FEASolver/file/test_file/parser_test.bdf";
  // 解析文件
  MAPPER::Parser parser;
  parser.ParserFile(addr);
  parser.DividedByComp();
  auto comp_datas = parser.GetCompsData();
  
  // 创建文件模型
  MODEL::Model model;

  // 文件映射到文件模型
  MAPPER::FileToMapper mapper;
  mapper.mapper(model,comp_datas);

  
  return 0;
}