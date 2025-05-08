#include "matrix_assemble.h"

#include "component/element/elemen_base.h"

namespace ASSEMBLE {

MatrixAssemble::MatrixAssemble(const MODEL::Model &model) {
  _model = model;
  for (auto elem : _model._element) {
    auto base_elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(elem);
    if (!base_elem) {
      throw "[ERROR]:func(MatrixAssemble)>>>有单元转换失败！";
    }
    _dof += 3 * base_elem->GetNodes().size();
  }
  _matrix_k = Eigen::MatrixXd::Zero(_dof, _dof);
  _vector_f = Eigen::VectorXd::Zero(_dof, 1);
  std::vector<std::string> xyz{"x", "y", "z"};
  for (int i = 0; i < _dof / 3; i++) {
    for (int j = 1; j < 4; j++) {
      std::string k = std::to_string(i) + "_" + xyz.at(j - 1);
      _dof2idx[k] = 3 * i + j;
    }
  }
}

void MatrixAssemble::AssembleK() {
  for (auto elem : _model._element) {
    auto base_elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(elem);
    if (!base_elem) {
      throw "[ERROR]:func(MatrixAssemble)>>>有单元转换失败！";
    }
    auto elem_matrix = base_elem->GetGlobalK(_model);

    // 获取单元节点号和单元刚度矩阵的维度
    auto em_col = elem_matrix.cols();
    auto em_row = elem_matrix.rows();
    if (em_col != em_row) {
      throw "[ERROR]:func(MatrixAssemble)>>>有单元刚度矩阵不是方阵！";
    }
    auto nodes = base_elem->GetNodes();

    // 创建单元刚度矩阵维度到自由度的映射关系
    std::map<int, std::string> elem_idx2dof;
    std::vector<std::string> xyz{"x", "y", "z"};
    for (int i = 0; i < nodes.size(); i++) {
      for (int j = 1; j < 4; j++) {
        auto k = std::to_string(nodes.at(i)) + "_" + xyz.at(j - 1);
        elem_idx2dof[3 * i + j] = k;
      }
    }

    // 将单元刚度矩阵组装到总体刚度矩阵上
    for (int r = 0; r < em_row; r++) {
      for (int c = 0; c < em_col; c++) {
        // 获取单元刚度矩阵元素对应的自由度
        std::string r_elem_dof = elem_idx2dof.at(r + 1);
        std::string c_elem_dof = elem_idx2dof.at(c + 1);
        // 获取总体刚度矩阵中该元素对应的位置
        int r_k_idx = _dof2idx.at(r_elem_dof);
        int c_k_idx = _dof2idx.at(c_elem_dof);
        // 将该元素加到总体刚度矩阵上
        _matrix_k(r_k_idx, c_k_idx) += elem_matrix(r + 1, c + 1);
      }
    }
  }
}




}  // namespace ASSEMBLE