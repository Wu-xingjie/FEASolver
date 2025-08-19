
#include "matrix_assemble/matrix_assemble.h"

#include <iostream>

#include "component/element/D1/ROD.h"
#include "component/element/D1/BAR.h"
#include "component/load/load_base.h"
#include "model_tool/display_matrixXd.h"
#include "mapper.h"
int main(int argv, char *argc[]) {
  if (argv != 2) {
    std::cout << "[ERROR]:func(main)>>> 输入参数错误!" << std::endl;
    return 1;
  }

  // 输入文件地址
  std::string inpfile_addr = argc[1];
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

  // 计算结果
  // std::cout << "_matrix_k:" << std::endl;
  // std::cout << matrix_assemble._matrix_k << std::endl;
  TOOL::DisplayMatrixXd(matrix_assemble._matrix_k, "total_stiffness_matrix",true);
  std::cout << "_vector_f:" << std::endl;
  std::cout << matrix_assemble._vector_f << std::endl;

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