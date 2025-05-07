#include "trans_coord_to_coord.h"

#include "cos_of_vectors.h"
namespace TOOL {
Eigen::Matrix3d TransCoordToCoord(
    const boost::shared_ptr<COMPONENT::CoordBase> base,
    const boost::shared_ptr<COMPONENT::CoordBase> obj) {
  Eigen::Matrix3d trans_matrix;
  // 获取两个坐标系的一般坐标系
  auto gen_base = base->GetGeneralCoord();
  auto gen_obj = obj->GetGeneralCoord();
  if (!gen_base._coord_flag && !gen_obj._coord_flag) {
    throw "[ERROR]: coord_to_coord函数中有一个坐标系没初始化!";
  }

  // 获取坐标点和坐标轴
  auto obj_origin = gen_obj._coord_origin;
  std::array<Eigen::Vector3d, 3> obj_axis;
  obj_axis[0] = gen_obj._vec1;
  obj_axis[1] = gen_obj._vec2;
  obj_axis[2] = gen_obj._vec3;

  auto base_origin = gen_base._coord_origin;
  std::array<Eigen::Vector3d, 3> base_axis;
  base_axis[0] = gen_base._vec1;
  base_axis[1] = gen_base._vec2;
  base_axis[2] = gen_base._vec3;

  // 计算坐标变换矩阵
  // clang-format off
    trans_matrix << TOOL::CosOfVecs(obj_axis.at(0),base_axis.at(0)),TOOL::CosOfVecs(obj_axis.at(1),base_axis.at(0)),TOOL::CosOfVecs(obj_axis.at(2),base_axis.at(0)),
                    TOOL::CosOfVecs(obj_axis.at(0),base_axis.at(1)),TOOL::CosOfVecs(obj_axis.at(1),base_axis.at(1)),TOOL::CosOfVecs(obj_axis.at(2),base_axis.at(1)),
                    TOOL::CosOfVecs(obj_axis.at(0),base_axis.at(2)),TOOL::CosOfVecs(obj_axis.at(1),base_axis.at(2)),TOOL::CosOfVecs(obj_axis.at(2),base_axis.at(2));
  // clang-format on

  return trans_matrix;
}

Eigen::Matrix3d TransCoordToCoord(
    const COMPONENT::GeneralCoord& base,
    const COMPONENT::GeneralCoord& obj) {
  Eigen::Matrix3d trans_matrix;
  if (!base._coord_flag && !obj._coord_flag) {
    throw "[ERROR]: coord_to_coord函数中有一个坐标系没初始化!";
  }

  // 获取坐标点和坐标轴
  auto obj_origin = obj._coord_origin;
  std::array<Eigen::Vector3d, 3> obj_axis;
  obj_axis[0] = obj._vec1;
  obj_axis[1] = obj._vec2;
  obj_axis[2] = obj._vec3;

  auto base_origin = base._coord_origin;
  std::array<Eigen::Vector3d, 3> base_axis;
  base_axis[0] = base._vec1;
  base_axis[1] = base._vec2;
  base_axis[2] = base._vec3;

  // 计算坐标变换矩阵
  // clang-format off
    trans_matrix << TOOL::CosOfVecs(obj_axis.at(0),base_axis.at(0)),TOOL::CosOfVecs(obj_axis.at(1),base_axis.at(0)),TOOL::CosOfVecs(obj_axis.at(2),base_axis.at(0)),
                    TOOL::CosOfVecs(obj_axis.at(0),base_axis.at(1)),TOOL::CosOfVecs(obj_axis.at(1),base_axis.at(1)),TOOL::CosOfVecs(obj_axis.at(2),base_axis.at(1)),
                    TOOL::CosOfVecs(obj_axis.at(0),base_axis.at(2)),TOOL::CosOfVecs(obj_axis.at(1),base_axis.at(2)),TOOL::CosOfVecs(obj_axis.at(2),base_axis.at(2));
  // clang-format on

  return trans_matrix;
}

}  // namespace TOOL