#include "ROD.h"

#include "geometry_tool/length_node_to_node.h"
#include "model_tool/get_comp_by_id.h"

namespace COMPONENT {

void ROD::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
}

void ROD::GenerateK(const MODEL::Model &model) {
  // 获取节点
  auto N1 = TOOL::GetCompById(model, _type, _G1);
  auto N2 = TOOL::GetCompById(model, _type, _G2);
}

Eigen::Matrix2d ROD::GetK() { return _loc_k; }

}  // namespace COMPONENT