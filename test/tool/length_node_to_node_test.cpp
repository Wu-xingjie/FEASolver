#include "geometry_tool/length_node_to_node.h"

#include "component/coordinate/cartesian_by_node.h"
#include "mapper.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "[ERROR]:输入参数个数有问题！" << std::endl;
    return 1;
  }
  // 解析文件
  MAPPER::Parser parser;
  parser.ParserFile(argv[1]);
  parser.DividedByComp();
  auto comp_datas = parser.GetCompsData();

  // 创建文件模型
  MODEL::Model model;

  // 文件映射到文件模型
  MAPPER::FileToMapper mapper;
  mapper.mapper(model, comp_datas);

  // 获取节点1
  auto base_node1 = model._node.at(1);
  auto node1 = boost::dynamic_pointer_cast<COMPONENT::Node>(base_node1);
  // 获取节点2
  auto base_node2 = model._node.at(3);
  auto node2 = boost::dynamic_pointer_cast<COMPONENT::Node>(base_node2);

  double l = TOOL::LenNode2Node(model, node1, node2);
  std::cout << "两节点之间的距离： " << l << std::endl;

  std::cout << "[INFO]:length_node_to_node测试通过!" << std::endl;
  return 0;
}