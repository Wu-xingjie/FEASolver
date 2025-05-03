#include "concentrated_force.h"

namespace COMPONENT {
void ConcentratedForce::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _nid = boost::any_cast<int>(card.at(2));
  _coord_id = boost::any_cast<int>(card.at(3));
  _scale = boost::any_cast<double>(card.at(4));
  _x = boost::any_cast<double>(card.at(5));
  _y = boost::any_cast<double>(card.at(6));
  _z = boost::any_cast<double>(card.at(7));
}

void ConcentratedForce::GenLoadVec(const MODEL::Model &model) {
  // 通过节点个数判断载荷列阵的维度
  int num_node = model._node.size();
  _load_vec = Eigen::VectorXd::Zero(num_node);
  // 设置载荷向量
  _load_vec[6 * num_node - 6] = _scale * _x;
  _load_vec[6 * num_node - 5] = _scale * _y;
  _load_vec[6 * num_node - 4] = _scale * _z;
  // 1x 1y 1z 1mx 1my 1mz 2x 2y 2z 2mx 2my 2mz 3x 3y 3z 3mx 3my 3mz
  // 0  1  2  3   4   5   6  7  8  9   10  11  12 13 14 15  16  17
  // 1x 2x 3x 
  // 0  6  12
}

} // namespace COMPONENT