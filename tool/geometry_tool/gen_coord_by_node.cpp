#include "gen_coord_by_node.h"

#include "is_nodes_collinear.h"
#include "model_tool/get_comp_by_id.h"
namespace TOOL {
boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model &model, const int &nid1, const int &nid2,
    const int &nid3) {
  auto gen_coord = boost::make_shared<COMPONENT::GeneralCoord>();
  // 一：通过节点获取各自对应的坐标系
  auto comp_n1 = GetCompById(model, COMPONENT::CompBase::comp_type::node, nid1);
  auto n1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_n1);
  if (!n1) {
    throw "[ERROR]:func(GetCompById)->获取节点1失败!";
  }
  auto comp_n2 = GetCompById(model, COMPONENT::CompBase::comp_type::node, nid1);
  auto n2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_n2);
  if (!n2) {
    throw "[ERROR]:func(GetCompById)->获取节点2失败!";
  }
  auto comp_n3 = GetCompById(model, COMPONENT::CompBase::comp_type::node, nid1);
  auto n3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_n3);
  if (!n3) {
    throw "[ERROR]:func(GetCompById)->获取节点3失败!";
  }
  auto comp_coord1 =
      GetCompById(model, COMPONENT::CompBase::comp_type::coord, n1->GetCoord());
  auto base_coord1 =
      boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord1);
  if (!base_coord1) {
    throw "[ERROR]: NodesToCoord中base_coord1转换失败!";
  }
  auto comp_coord2 =
      GetCompById(model, COMPONENT::CompBase::comp_type::coord, n2->GetCoord());
  auto base_coord2 =
      boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord2);
  if (!base_coord2) {
    throw "[ERROR]: NodesToCoord中base_coord2转换失败!";
  }
  auto comp_coord3 =
      GetCompById(model, COMPONENT::CompBase::comp_type::coord, n3->GetCoord());
  auto base_coord3 =
      boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord3);
  if (!base_coord3) {
    throw "[ERROR]: NodesToCoord中base_coord3转换失败!";
  }

  if (!base_coord1->GetGeneralCoord()._coord_flag &&
      !base_coord2->GetGeneralCoord()._coord_flag &&
      !base_coord3->GetGeneralCoord()._coord_flag) {
    throw "[ERROR]: NodesToCoord中有节点参考坐标系未创建";
  }

  // 二：将各个节点变换到全局坐标系下
  auto loc_n1 = n1->get_location();
  auto loc_n2 = n2->get_location();
  auto loc_n3 = n3->get_location();
  auto global_coord = boost::make_shared<COMPONENT::GlobalCoord>();
  auto trans_matrix_n1 = TransCoordToCoord(global_coord, base_coord1);
  auto trans_matrix_n2 = TransCoordToCoord(global_coord, base_coord2);
  auto trans_matrix_n3 = TransCoordToCoord(global_coord, base_coord3);

  auto glob_n1 = trans_matrix_n1 * loc_n1 + base_coord1->getORIGIN();
  auto glob_n2 = trans_matrix_n2 * loc_n2 + base_coord2->getORIGIN();
  auto glob_n3 = trans_matrix_n3 * loc_n3 + base_coord3->getORIGIN();

  std::vector<Eigen::Vector3d> nodes{glob_n1, glob_n2, glob_n3};
  if (IsNodesCollinear(nodes)) {
    throw "[ERROR]: func: NodesToCoord->三点共线！";
  }

  // 三：通过全局坐标系下的三点坐标获得一般坐标系
  auto axis_z = glob_n2 - glob_n1;
  auto vec_n1_n3 = glob_n3 - glob_n1;
  // 保证glob_n3始终在生成的一般坐标系的x正半轴
  double n3_pos = glob_n3.x() - glob_n1.x();
  Eigen::Vector3d axis_y;
  if (n3_pos > 1.0e-11) {
    axis_y = axis_z.cross(vec_n1_n3);
  } else {
    axis_y = vec_n1_n3.cross(axis_z);
  }
  auto axis_x = axis_z.cross(axis_y);
  // 给一般坐标系赋值
  gen_coord->_coord_origin = glob_n1;
  gen_coord->_vec1 = axis_x;
  gen_coord->_vec2 = axis_y;
  gen_coord->_vec3 = axis_z;
  gen_coord->_dim_type = COMPONENT::GeneralCoord::gen_coord_type::dim3;
  return gen_coord;
}

boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model &model, const int &nid1, const int &nid2) {
  auto gen_coord = boost::make_shared<COMPONENT::GeneralCoord>();
  // 一：通过节点获取各自对应的坐标系
  auto comp_n1 = GetCompById(model, COMPONENT::CompBase::comp_type::node, nid1);
  auto n1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_n1);
  if (!n1) {
    throw "[ERROR]:func(GetCompById)->获取节点1失败!";
  }
  auto comp_n2 = GetCompById(model, COMPONENT::CompBase::comp_type::node, nid2);
  auto n2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_n2);
  if (!n2) {
    throw "[ERROR]:func(GetCompById)->获取节点2失败!";
  }
  auto comp_coord1 =
      GetCompById(model, COMPONENT::CompBase::comp_type::coord, n1->GetCoord());
  auto base_coord1 =
      boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord1);
  if (!base_coord1) {
    throw "[ERROR]: NodesToCoord中base_coord1转换失败!";
  }
  auto comp_coord2 =
      GetCompById(model, COMPONENT::CompBase::comp_type::coord, n2->GetCoord());
  auto base_coord2 =
      boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord2);
  if (!base_coord2) {
    throw "[ERROR]: NodesToCoord中base_coord2转换失败!";
  }

  if (!base_coord1->GetGeneralCoord()._coord_flag &&
      !base_coord2->GetGeneralCoord()._coord_flag) {
    throw "[ERROR]: NodesToCoord中有节点参考坐标系未创建";
  }

  // 二：将各个节点变换到全局坐标系下
  auto loc_n1 = n1->get_location();
  auto loc_n2 = n2->get_location();
  auto global_coord = boost::make_shared<COMPONENT::GlobalCoord>();
  auto trans_matrix_n1 = TransCoordToCoord(global_coord, base_coord1);
  auto trans_matrix_n2 = TransCoordToCoord(global_coord, base_coord2);

  auto glob_n1 = trans_matrix_n1 * loc_n1 + base_coord1->getORIGIN();
  auto glob_n2 = trans_matrix_n2 * loc_n2 + base_coord2->getORIGIN();

  // 三：通过全局坐标系下的获得一般坐标系
  gen_coord->_coord_origin = glob_n1;
  gen_coord->_vec1 = glob_n2 - glob_n1;
  gen_coord->_vec2 << 0, 0, 0;
  gen_coord->_vec3 << 0, 0, 0;
  gen_coord->_dim_type = COMPONENT::GeneralCoord::gen_coord_type::dim1;
  return gen_coord;
}

}  // namespace TOOL