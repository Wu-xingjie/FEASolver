#include "one_dim_storage.h"
#include "component/element/elemen_base.h"
#include "model_tool/get_elem_idx2dof_map.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <set>

namespace ASSEMBLE {
OneDimMatrixAssemble::OneDimMatrixAssemble(const MODEL::Model &model) {
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
  std::vector<std::string> xyz{"vx", "vy", "vz", "rx", "ry", "rz"};
  std::vector<std::string> dof_used;
  for (int i = 1; i < _node_used.size() + 1; i++) {
    for (int j = 0; j < 6; j++) {
      std::string k = std::to_string(vec_node_used.at(i - 1)) + "_" + xyz.at(j);
      dof_used.push_back(k);
      _dof2idx.insert({k, 6 * (i - 1) + j});
    }
  }

  // 定义_dof2node
  for (auto &dof : dof_used) {
    _dof2node[dof] = FindNodesOfDof(dof);
  }

  // 初始化_matrix_k
  int estimate_size = 0;
  for (auto &dof : dof_used) {
    auto nodes = _dof2node.at(dof);
    auto min_node = std::min_element(nodes.begin(), nodes.end());
    auto min_node_dof = std::to_string(*min_node) + "_vx";
    int diag_dof_idx = _dof2idx.left.at(dof);
    int min_node_idx = _dof2idx.left.at(min_node_dof);
    // 创建_matrix_k中的自由度到下标的双向映射表
    auto min_dof_idx =
        std::find(dof_used.cbegin(), dof_used.cend(), min_node_dof);
    auto diag_idx = std::find(dof_used.cbegin(), dof_used.cend(), dof);
    if (min_dof_idx == dof_used.end() || diag_idx == dof_used.end()) {
      throw std::runtime_error(
          "[ERROR]:func(OneDimMatrixAssemble::OneDimMatrixAssemble)>>>"
          "未在使用到的自由度列表(used_dof)中搜索到对应元素！");
    }
    int temp_len = 0;
    for (auto ptr = min_dof_idx; ptr != diag_idx + 1; ptr++) {
      std::array<std::string, 2> dof_pair{*ptr, dof};
      _matrix_k._dof2idx.insert({dof_pair, estimate_size + temp_len});
      temp_len += 1;
    }
    _matrix_k._diag_elem_loc.Zero(estimate_size);
    // 计算_matrix_k._none_zero_elem大概长度
    estimate_size += diag_dof_idx - min_node_idx + 1;
  }
  _matrix_k._none_zero_elem.Zero(estimate_size);
  _vector_f = Eigen::VectorXd::Zero(_dof);
}

std::vector<int> OneDimMatrixAssemble::FindNodesOfDof(const std::string &dof) {
  std::vector<int> result;
  for (auto elem : _model._element) {
    auto base_elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(elem);
    if (!base_elem) {
      throw "[ERROR]:func(OneDimMatrixAssemble::FindNodesOfDof)>>>"
            "有单元转换失败！";
    }
    auto elem_nodes = base_elem->GetNodes();
    // 提取该自由度节点编号
    auto idx_lab = dof.find_first_of("_");
    if (idx_lab == std::string::npos) {
      throw "[ERROR]:func(OneDimMatrixAssemble::FindNodesOfDof)>>>"
            "自由度dof中未找到下划线！";
    }
    std::string str_node = dof.substr(idx_lab);
    int int_dof = boost::lexical_cast<int>(str_node);

    if (std::find(elem_nodes.cbegin(), elem_nodes.cend(), int_dof) !=
        elem_nodes.cend()) {
      result.insert(result.end(), elem_nodes.begin(), elem_nodes.end());
    }
  }
  return result;
}

void OneDimMatrixAssemble::AssembleK() {
  for (auto elem : _model._element) {
    auto base_elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(elem);
    if (!base_elem) {
      throw "[ERROR]:func(MatrixAssemble)>>>有单元转换失败！";
    }
    auto elem_matrix = base_elem->GetGlobalK(_model);
    auto elem_idx2dof = TOOL::ElemIdx2Dof(*base_elem);
    // 通过单刚矩阵索引坐标 -> 自由度坐标 -> 一位数组索引的映射，给_matrix_k赋值
    for (int r = 0; r < elem_matrix.size(); r++) {
      bool begin_store = false;
      for (int c = 0; c < r + 1; c++) {
        // 找到第一个非零元素然后开始存储
        if (std::abs(elem_matrix(r, c)) > 1.0e-16) {
          begin_store = true;
        }
        if (begin_store) {
          auto dof_coord = elem_idx2dof.at({r, c});
          int array_idx = _matrix_k._dof2idx.left.at(dof_coord);
          _matrix_k._none_zero_elem(array_idx) += elem_matrix(r, c);
        }
      }
    }
  }
}

} // namespace ASSEMBLE