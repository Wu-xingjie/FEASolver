#include "concentrated_force.h"
#include "component/coordinate/coordinate_base.h"
#include "geometry_tool/trans_coord_to_coord.h"
#include "model_tool/get_comp_by_id.h"
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

Eigen::VectorXd ConcentratedForce::GetGLobalLoad(const MODEL::Model &model) {
  // 载荷坐标系
  auto comp_coord = TOOL::GetCompById(
      model, COMPONENT::CompBase::comp_type::coord, _coord_id);
  auto base_coord = boost::dynamic_pointer_cast<CoordBase>(comp_coord);
  if (!base_coord) {
    throw "[ERROR]:func(GetGLobalLoad):无法通过坐标系id找到对应坐标系!";
  }
  // 将载荷坐标系变换到全局坐标系下
  auto base_global_coord = model._coord.front();
  auto global_coord = boost::dynamic_pointer_cast<CoordBase>(base_global_coord);
  auto trans_matrix = TOOL::TransCoordToCoord(global_coord, base_coord);
  // 获取全局坐标系下的载荷列阵
  Eigen::Vector3d origin;
  origin << _x, _y, _z;
  return trans_matrix * (_scale * origin);
}

} // namespace COMPONENT