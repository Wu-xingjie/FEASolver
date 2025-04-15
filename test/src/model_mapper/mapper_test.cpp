#include "mapper.h"

#include "component/element/elemen_base.h"
#include "component/node/node.h"

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
  mapper.mapper(model, comp_datas);

  auto test_node = model._node.front();
  auto node = boost::dynamic_pointer_cast<COMPONENT::Node>(test_node);
  std::cout << "node id: " << node->GetID() << std::endl;

  auto test_elem = model._element.front();
  auto elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(test_elem);
  std::cout << "elem id: " << elem->GetID() << std::endl;

  return 0;
}