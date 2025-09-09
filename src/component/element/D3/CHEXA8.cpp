#include "CHEXA8.h"

#include <cmath>
#include <exception>

#include "component/coordinate/global_coordinate.h"
#include "geometry_tool/area_of_three_points.h"
#include "geometry_tool/gen_coord_by_node.h"
#include "geometry_tool/length_node_to_node.h"
#include "math_tool/area_func_expr.h"
#include "math_tool/func_expr/func_cal.h"
#include "math_tool/gauss_integral.h"
#include "model_tool/deal_E_NU_G.h"
#include "model_tool/display_matrixXd.h"
#include "model_tool/get_comp_by_id.h"

namespace COMPONENT {

Chexa8::Chexa8() {
  _elem_type = ElemBase::elem_type::chexa8;
  _loc_k = Eigen::MatrixXd::Zero(24, 24);
}

void Chexa8::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
  _G3 = boost::any_cast<int>(card.at(5));
  _G4 = boost::any_cast<int>(card.at(6));
  _G5 = boost::any_cast<int>(card.at(7));
  _G6 = boost::any_cast<int>(card.at(8));
  _G7 = boost::any_cast<int>(card.at(9));
  _G8 = boost::any_cast<int>(card.at(10));
}

double Chexa8::LenOfNode(const Eigen::Vector2d &n1, const Eigen::Vector2d &n2) {
  double num0 = std::pow(n1[0] - n2[0], 2);
  double num1 = std::pow(n1[1] - n2[1], 2);
  return std::pow(num0 + num1, 0.5);
}

void Chexa8::GenerateK(const MODEL::Model &model) {}

Eigen::MatrixXd Chexa8::GetGlobalK(const MODEL::Model &model) {
  // 获取全局坐标系
  auto global_coord =
      boost::make_shared<COMPONENT::GlobalCoord>()->GetGeneralCoord();
  // 构建局部坐标系
  // 获取节点
  auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
  auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
  if (!N1) {
    throw std::runtime_error("[ERROR]:func(Chexa8::GenerateK)>>>节点1获取失败");
  }
  auto N1_datas = N1->get_location();
  auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
  auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
  if (!N2) {
    throw std::runtime_error("[ERROR]:func(Chexa8::GenerateK)>>>节点2获取失败");
  }
  auto N2_datas = N2->get_location();
  auto comp_N3 = TOOL::GetCompById(model, CompBase::comp_type::node, _G3);
  auto N3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N3);
  if (!N3) {
    throw std::runtime_error("[ERROR]:func(Chexa8::GenerateK)>>>节点3获取失败");
  }
  auto N3_datas = N3->get_location();
  auto comp_N4 = TOOL::GetCompById(model, CompBase::comp_type::node, _G4);
  auto N4 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N4);
  if (!N4) {
    throw std::runtime_error("[ERROR]:func(Chexa8::GenerateK)>>>节点4获取失败");
  }
  auto N4_datas = N4->get_location();

  auto n1_vec = Eigen::Vector2d{N1_datas(0), N1_datas(1)};
  auto n2_vec = Eigen::Vector2d{N2_datas(0), N2_datas(1)};
  auto n3_vec = Eigen::Vector2d{N3_datas(0), N3_datas(1)};
  auto n4_vec = Eigen::Vector2d{N4_datas(0), N4_datas(1)};
  // 计算单元长和宽
  auto length = LenOfNode(n1_vec, n2_vec);
  auto weight = LenOfNode(n4_vec, n3_vec);

  // 单元中点坐标
  Eigen::Vector3d mid_point = Eigen::Vector3d::Zero(3);
  mid_point(0) = (N1_datas(0) + N2_datas(0) + N3_datas(0) + N4_datas(0)) / 4;
  mid_point(1) = (N1_datas(1) + N2_datas(1) + N3_datas(1) + N4_datas(1)) / 4;
  mid_point(2) = (N1_datas(2) + N2_datas(2) + N3_datas(2) + N4_datas(2)) / 4;

  // 计算局部坐标系坐标轴
  auto loc_axis_x = TOOL::NodesToCoord(model, _G1, _G2)->_vec1;
  auto loc_axis_y = TOOL::NodesToCoord(model, _G1, _G4)->_vec1;
  auto loc_axis_z = loc_axis_x.cross(loc_axis_y);

  GeneralCoord loc_coord;
  loc_coord._coord_origin = mid_point;
  loc_coord._dim_type = GeneralCoord::gen_coord_type::dim3;
  loc_coord._vec1 = loc_axis_x.normalized();
  loc_coord._vec2 = loc_axis_y.normalized();
  loc_coord._vec3 = loc_axis_z.normalized();

  // 生成坐标变换矩阵
  auto trans_matrix_block = TOOL::TransCoordToCoord(global_coord, loc_coord);

  // std::cout << "坐标变换矩阵:" << std::endl;
  // TOOL::DisplayMatrixXd(trans_matrix_block);

  Eigen::MatrixXd trans_matrix = Eigen::MatrixXd::Zero(24, 24);
  trans_matrix.block<3, 3>(0, 0) = trans_matrix_block;
  trans_matrix.block<3, 3>(3, 3) = trans_matrix_block;
  trans_matrix.block<3, 3>(6, 6) = trans_matrix_block;
  trans_matrix.block<3, 3>(9, 9) = trans_matrix_block;
  trans_matrix.block<3, 3>(12, 12) = trans_matrix_block;
  trans_matrix.block<3, 3>(15, 15) = trans_matrix_block;
  trans_matrix.block<3, 3>(18, 18) = trans_matrix_block;
  trans_matrix.block<3, 3>(21, 21) = trans_matrix_block;

  // 生成全局坐标系下的单元刚度矩阵
  // 全局坐标系下单元刚度矩阵
  Eigen::MatrixXd global_k;
  // TOOL::DisplayMatrixXd(trans_matrix, "trans_matrix", true);
  global_k = trans_matrix * _loc_k * trans_matrix.transpose();

  // TOOL::DisplayMatrixXd(global_k, "global_k");
  return global_k;
}
} // namespace COMPONENT