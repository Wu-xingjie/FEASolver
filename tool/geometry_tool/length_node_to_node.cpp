#include "length_node_to_node.h"
#include "geometry_tool/cos_of_vectors.h"
namespace TOOL {
double LenNode2Node(const COMPONENT::Node &n1, const COMPONENT::Node &n2) {
  double result = 0;
  COMPONENT::vec_3 loc_n1 = n1.get_location();
  COMPONENT::vec_3 loc_n2 = n2.get_location();
  // 两节点在同一坐标系下时
  if (n1.GetCoord() == n2.GetCoord()) {
    COMPONENT::vec_3 vec_n1_to_n2(loc_n2(0) - loc_n1(0), loc_n2(1) - loc_n1(1),
                                  loc_n2(2) - loc_n1(2));
    result = vec_n1_to_n2.norm();
  } else {
    // 两节点不在同一坐标系上时，先将两个节点变换到全局坐标系上，再计算节点间距离
    // 设置全局坐标系
    COMPONENT::vec_3 globle_x(1, 0, 0);
    COMPONENT::vec_3 globle_y(0, 1, 0);
    COMPONENT::vec_3 globle_z(0, 0, 1);
    // 把n1变换到全局坐标系下
    std::array<COMPONENT::vec_3, 3> n1_coord = n1.GetCoord().getVEC();
    COMPONENT::vec_3 n1_coord_origin = n1.GetCoord().getORIGIN();
    Eigen::Matrix3d trans_matrix1;

    // clang-format off
    trans_matrix1<<TOOL::CosOfVecs(n1_coord.at(0),globle_x),TOOL::CosOfVecs(n1_coord.at(1),globle_x),TOOL::CosOfVecs(n1_coord.at(2),globle_x),
                   TOOL::CosOfVecs(n1_coord.at(0),globle_y),TOOL::CosOfVecs(n1_coord.at(1),globle_y),TOOL::CosOfVecs(n1_coord.at(2),globle_y),
                   TOOL::CosOfVecs(n1_coord.at(0),globle_z),TOOL::CosOfVecs(n1_coord.at(1),globle_z),TOOL::CosOfVecs(n1_coord.at(2),globle_z);
    // clang-format on
    COMPONENT::vec_3 globle_n1 = n1_coord_origin + trans_matrix1 * loc_n1;

    //  把n2变换到全局坐标系下
    std::array<COMPONENT::vec_3, 3> n2_coord = n2.GetCoord().getVEC();
    COMPONENT::vec_3 n2_coord_origin = n2.GetCoord().getORIGIN();
    Eigen::Matrix3d trans_matrix2;

    // clang-format off
    trans_matrix2<<TOOL::CosOfVecs(n2_coord.at(0),globle_x),TOOL::CosOfVecs(n2_coord.at(1),globle_x),TOOL::CosOfVecs(n2_coord.at(2),globle_x),
                   TOOL::CosOfVecs(n2_coord.at(0),globle_y),TOOL::CosOfVecs(n2_coord.at(1),globle_y),TOOL::CosOfVecs(n2_coord.at(2),globle_y),
                   TOOL::CosOfVecs(n2_coord.at(0),globle_z),TOOL::CosOfVecs(n2_coord.at(1),globle_z),TOOL::CosOfVecs(n2_coord.at(2),globle_z);
    // clang-format on
    COMPONENT::vec_3 globle_n2 = n2_coord_origin + trans_matrix2 * loc_n2;

    // 在全局坐标系下求出两节点间的距离
    COMPONENT::vec_3 vec_n1_to_n2(globle_n1(0) - globle_n2(0),
                                  globle_n1(1) - globle_n2(1),
                                  globle_n1(2) - globle_n2(2));
    result = vec_n1_to_n2.norm();
  }
  return result;
}
} // namespace TOOL