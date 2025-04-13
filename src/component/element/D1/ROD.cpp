#include "ROD.h"
#include "geometry_tool/length_node_to_node.h"

namespace COMPONENT {

void ROD::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card[1]);
  _pid = boost::any_cast<int>(card[2]);
  _G1 = boost::any_cast<int>(card[3]);
  _G2 = boost::any_cast<int>(card[4]);
}

// void GenerateK(const MODEL::Model &model) {
//   std::cout << "暂未处理杆单元刚度矩阵" << std::endl;
// }

Eigen::Matrix2d ROD::GetK() { return _loc_k; }

} // namespace COMPONENT