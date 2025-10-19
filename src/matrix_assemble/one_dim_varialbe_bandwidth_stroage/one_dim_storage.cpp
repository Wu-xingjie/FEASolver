#include "one_dim_storage.h"
#include "component/element/elemen_base.h"
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

} // namespace ASSEMBLE