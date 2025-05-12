
#include "matrix_assemble/matrix_assemble.h"

#include <iostream>

#include "component/element/D1/ROD.h"
#include "component/load/load_base.h"
#include "mapper.h"
int main() {
  // 输入文件地址
  std::string inpfile_addr =
      "/home/wxj/workspace/FEASolver/file/test_file/parser_test.bdf";
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
  // 生成单元刚度矩阵和载荷列阵
  for (auto comp_elem : model._element) {
    auto base_elem =
        boost::dynamic_pointer_cast<COMPONENT::ElemBase>(comp_elem);
    base_elem->GenerateK(model);
  }
  for (auto comp_load : model._load) {
    auto base_load =
        boost::dynamic_pointer_cast<COMPONENT::LoadBase>(comp_load);
    base_load->GenLoadVec(model);
  }
  // 矩阵组装
  ASSEMBLE::MatrixAssemble matrix_assemble(model);
  matrix_assemble.AssembleK();
  matrix_assemble.AssembleLoad();
  matrix_assemble.AddConstrain();

  //   计算结果

  // Eigen::Matrix4d test;
  // test << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 65.9736;
  // Eigen::Vector4d test_f;
  // test_f << 0, 0, 0, 1000;
  // std::cout << "test:" << std::endl;
  // std::cout << test << std::endl;
  // std::cout << "inverse test:" << std::endl;
  // std::cout << test.inverse() << std::endl;
  // std::cout << "test节点位移:" << std::endl;
  // std::cout << test.inverse() * test_f << std::endl;

  matrix_assemble.GetExtraDof();
  auto K = matrix_assemble.RemoveExtraMatrixDof();
  auto f = matrix_assemble.RemoveExtraLoadDof();
  std::cout << "K:" << std::endl;
  std::cout << K << std::endl;
  std::cout << "inverse K:" << std::endl;
  std::cout << K.inverse() << std::endl;
  std::cout << "f:" << std::endl;
  std::cout << f << std::endl;
  auto result = K.inverse() * f;
  std::cout << "节点位移:" << std::endl;
  std::cout << result << std::endl;

  return 0;
}