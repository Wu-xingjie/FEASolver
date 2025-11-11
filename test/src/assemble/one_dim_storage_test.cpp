// #include "matrix_assemble/matrix_assemble.h"
// #include "matrix_assemble/one_dim_varialbe_bandwidth_stroage/one_dim_storage.h"

#include <iostream>

// #include "component/element/D1/BAR.h"
// #include "component/element/D1/ROD.h"
// #include "component/load/load_base.h"
// #include "mapper.h"
// #include "model_tool/display_matrixXd.h"

int main(int argv, char *argc[]) {
//   if (argv != 2) {
//     std::cout << "[ERROR]:func(main)>>> 输入参数错误!" << std::endl;
//     return 1;
//   }

//   // 输入文件地址
//   std::string inpfile_addr = argc[1];
//   // 解析文件
//   MAPPER::Parser parser;
//   parser.ParserFile(inpfile_addr);
//   parser.DividedByComp();
//   auto comp_datas = parser.GetCompsData();
//   // 创建文件模型
//   MODEL::Model model;
//   // 文件映射到文件模型
//   MAPPER::FileToMapper mapper;
//   mapper.mapper(model, comp_datas);
//   // 生成单元刚度矩阵和载荷列阵
//   for (auto comp_elem : model._element) {
//     auto base_elem =
//         boost::dynamic_pointer_cast<COMPONENT::ElemBase>(comp_elem);
//     base_elem->GenerateK(model);
//   }
//   for (auto comp_load : model._load) {
//     auto base_load =
//         boost::dynamic_pointer_cast<COMPONENT::LoadBase>(comp_load);
//     base_load->GenLoadVec(model);
//   }

//   // 全矩阵组装
//   // 矩阵组装
//   ASSEMBLE::MatrixAssemble matrix_assemble(model);
//   matrix_assemble.AssembleK();
//   TOOL::DisplayMatrixXd(matrix_assemble._matrix_k, "total_stiffness_matrix",
//                         true);

//   // 矩阵一维变带宽组装
//   ASSEMBLE::OneDimMatrixAssemble test_assmebler(model);
//   test_assmebler.AssembleK();
//   test_assmebler.ShowK();

  return 0;
}