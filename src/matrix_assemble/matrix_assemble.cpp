#include "matrix_assemble.h"

#include <boost/lexical_cast.hpp>
#include <set>

#include "component/constrained/single_point_constrained.h"
#include "component/element/elemen_base.h"
#include "component/load/load_base.h"
#include "matrix_tool/delete_row_or_col.h"
namespace ASSEMBLE {

MatrixAssemble::MatrixAssemble(const MODEL::Model& model) {
  _model = model;

  std::set<int> _node_used;
  for (auto elem : _model._element) {
    auto base_elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(elem);
    if (!base_elem) {
      throw "[ERROR]:func(MatrixAssemble)>>>有单元转换失败！";
    }
    auto ns = base_elem->GetNodes();
    for (auto n : ns) {
      _node_used.insert(n);
    }
  }
  _dof += 6 * _node_used.size();
  std::vector<int> vec_node_used;
  for (auto elem : _node_used) {
    vec_node_used.push_back(elem);
  }
  _matrix_k = Eigen::MatrixXd::Zero(_dof, _dof);
  _vector_f = Eigen::VectorXd::Zero(_dof, 1);
  std::vector<std::string> xyz{"vx", "vy", "vz", "rx", "ry", "rz"};
  for (int i = 1; i < _node_used.size() + 1; i++) {
    for (int j = 0; j < 6; j++) {
      std::string k = std::to_string(vec_node_used.at(i - 1)) + "_" + xyz.at(j);
      // _dof2idx[k] = 6 * (i - 1) + j;
      _dof2idx.insert({k, 6 * (i - 1) + j});
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
    std::vector<std::string> xyz{"vx", "vy", "vz", "rx", "ry", "rz"};
    for (int i = 1; i < nodes.size() + 1; i++) {
      for (int j = 0; j < 6; j++) {
        auto k = std::to_string(nodes.at(i - 1)) + "_" + xyz.at(j);
        elem_idx2dof[6 * (i - 1) + j] = k;
      }
    }

    // 将单元刚度矩阵组装到总体刚度矩阵上
    for (int r = 0; r < em_row; r++) {
      for (int c = 0; c < em_col; c++) {
        // 获取单元刚度矩阵元素对应的自由度
        std::string r_elem_dof = elem_idx2dof.at(r);
        std::string c_elem_dof = elem_idx2dof.at(c);
        // 获取总体刚度矩阵中该元素对应的位置
        int r_k_idx = _dof2idx.left.find(r_elem_dof)->second;
        int c_k_idx = _dof2idx.left.find(c_elem_dof)->second;
        // 将该元素加到总体刚度矩阵上
        _matrix_k(r_k_idx, c_k_idx) += elem_matrix(r, c);
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
    std::cout << "global_load: " << std::endl << global_load << std::endl;
    auto nodes = base_load->GetNodes();

    // 创建载荷列阵维度到自由度的映射关系
    std::map<int, std::string> load_idx2dof;
    std::vector<std::string> xyz{"vx", "vy", "vz", "rx", "ry", "rz"};
    for (int i = 0; i < nodes.size(); i++) {
      for (int j = 1; j < 7; j++) {
        auto val = std::to_string(nodes.at(i)) + "_" + xyz.at(j - 1);
        load_idx2dof[6 * i + j] = val;
      }
    }
    // 给全局载荷列阵赋值
    for (int i = 0; i < global_load.size(); i++) {
      std::string dof_load = load_idx2dof.at(i + 1);
      int global_load_idx = _dof2idx.left.find(dof_load)->second;
      _vector_f(global_load_idx) += global_load(i);
    }
  }
  std::cout << "_vector_f: " << std::endl << _vector_f << std::endl;
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
    std::vector<std::string> xyz{"vx", "vy", "vz", "rx", "ry", "rz"};
    std::vector<std::string> constrianed_dof;
    for (auto spc : vec_spc) {
      for (auto elem : spc._component) {
        int xyz_idx = boost::lexical_cast<int>(elem);
        std::string dof = std::to_string(spc._node) + "_" + xyz.at(xyz_idx - 1);
        constrianed_dof.push_back(dof);
      }
    }

    // 置0法：
    // 将总体刚度矩阵中被约束自由度对应行和列的非对角元素设为0,对角元素设置为1
    // 载荷列阵中对应自由度元素设置为0
    for (auto dof : constrianed_dof) {
      int idx = _dof2idx.left.find(dof)->second;
      _matrix_k.row(idx).setZero();
      _matrix_k.col(idx).setZero();
      _matrix_k(idx, idx) = 1;
      _vector_f(idx) = 0;
    }
  }
}

Eigen::MatrixXd MatrixAssemble::RemoveExtraMatrixDof() {
  // 去除刚度矩阵多余自由度
  Eigen::MatrixXd result;
  result = TOOL::DelRowOrCol(TOOL::RowOrCol::both, _extro_dof, _matrix_k);
  return result;
}

Eigen::VectorXd MatrixAssemble::RemoveExtraLoadDof() {
  // 去除载荷列阵多余自由度
  Eigen::VectorXd result;
  result = TOOL::DelRowOrCol(_extro_dof, _vector_f);
  return result;
}

void MatrixAssemble::GetExtraDof() {
  int dof = _matrix_k.cols();
  for (int i = 0; i < dof; i++) {
    bool is_extro = true;
    auto col = _matrix_k.col(i);
    for (auto elem : col) {
      if (std::abs(elem) > 1e-16) {
        is_extro = false;
      }
    }
    if (is_extro) {
      _extro_dof.insert(i);
    }
  }
}

std::vector<std::string> MatrixAssemble::OutputValidDofSerial() {
  std::vector<std::string> result;
  for (int i = 0; i < _dof; i++) {
    auto pos = _extro_dof.find(i);
    if (pos != _extro_dof.end()) {
      continue;
    }
    result.push_back(_dof2idx.right.find(i)->second);
  }

  return result;
}

}  // namespace ASSEMBLE