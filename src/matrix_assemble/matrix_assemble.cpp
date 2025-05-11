#include "matrix_assemble.h"
#include "component/constrained/single_point_constrained.h"
#include "component/element/elemen_base.h"
#include "component/load/load_base.h"

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

void MatrixAssemble::AssembleLoad() {
  for (auto load : _model._load) {
    // 获取载荷
    auto base_load = boost::dynamic_pointer_cast<COMPONENT::LoadBase>(load);
    if (!base_load) {
      throw "[ERROR]:func(AssembleLoad): 无法获得载荷！";
    }
    auto global_load = base_load->GetGLobalLoad(_model);
    auto nodes = base_load->GetNodes();

    // 创建单元刚度矩阵维度到自由度的映射关系
    std::map<int, std::string> load_idx2dof;
    std::vector<std::string> xyz{"x", "y", "z"};
    for (int i = 0; i < nodes.size(); i++) {
      for (int j = 1; j < 4; j++) {
        auto val = std::to_string(nodes.at(i)) + "_" + xyz.at(j - 1);
        load_idx2dof[3 * i + j] = val;
      }
    }
    // 给全局载荷列阵赋值
    for (int i = 0; i < global_load.rows(); i++) {
      std::string dof_load = load_idx2dof.at(i + 1);
      int global_load_idx = _dof2idx.at(dof_load);
      _vector_f(global_load_idx) += global_load(i);
    }
  }
}

void MatrixAssemble::AddConstrain() {
  for (auto constrain : _model._constrain) {
    // 获取约束
    auto base_constrain =
        boost::dynamic_pointer_cast<COMPONENT::SinglePointConstrains>(
            constrain);
    if (!base_constrain) {
      throw "[ERROR]:func(AddConstrain): 无法获取约束";
    }

    // 获取被约束的自由度
    auto vec_spc = base_constrain->GetConstrain();
    std::vector<std::string> constrianed_dof;
    for (auto spc : vec_spc) {
      for (auto elem : spc._component) {
        std::string dof = std::to_string(spc._node) + "_" + elem;
        constrianed_dof.push_back(dof);
      }
    }

    // 置0法：
    // 将总体刚度矩阵中被约束自由度对应行和列的非对角元素设为0,对角元素设置为1
    // 载荷列阵中对应自由度元素设置为0
    for (auto dof : constrianed_dof) {
      int idx = _dof2idx.at(dof);
      _matrix_k.row(idx).setZero();
      _matrix_k.col(idx).setZero();
      _matrix_k(idx, idx) = 1;
      _vector_f(idx) = 0;
    }
  }
}

} // namespace ASSEMBLE