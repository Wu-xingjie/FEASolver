#include "CQUAD4_kirchhoff.h"

#include <cmath>
#include <exception>

#include "component/coordinate/global_coordinate.h"
#include "geometry_tool/area_of_three_points.h"
#include "geometry_tool/gen_coord_by_node.h"
#include "geometry_tool/length_node_to_node.h"
#include "math_tool/area_func_expr.h"
#include "math_tool/func_expr/func_cal.h"
#include "math_tool/tri_gauss_integral.h"
#include "model_tool/deal_E_NU_G.h"
#include "model_tool/get_comp_by_id.h"

namespace COMPONENT {

Cquad4Kf::Cquad4Kf() {
  _elem_type = ElemBase::elem_type::cquad4_kf;
  _loc_k = Eigen::MatrixXd::Zero(12, 12);
}

void Cquad4Kf::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
  _G3 = boost::any_cast<int>(card.at(5));
  _G4 = boost::any_cast<int>(card.at(6));
}

double Cquad4Kf::LenOfNode(const Eigen::Vector2d &n1,
                           const Eigen::Vector2d &n2) {
  double num0 = std::pow(n1[0] - n2[0], 2);
  double num1 = std::pow(n1[1] - n2[1], 2);
  return std::pow(num0 + num1, 0.5);
}

void Cquad4Kf::GenerateK(const MODEL::Model &model) {
  try {
    // 获取节点
    auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();
    auto comp_N3 = TOOL::GetCompById(model, CompBase::comp_type::node, _G3);
    auto N3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N3);
    if (!N3) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>节点3获取失败");
    }
    auto N3_datas = N3->get_location();
    auto comp_N4 = TOOL::GetCompById(model, CompBase::comp_type::node, _G4);
    auto N4 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N4);
    if (!N4) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>节点4获取失败");
    }
    auto N4_datas = N4->get_location();

    // 计算单元面积
    auto n1_vec = Eigen::Vector2d{N1_datas(0), N1_datas(1)};
    auto n2_vec = Eigen::Vector2d{N2_datas(0), N2_datas(1)};
    auto n3_vec = Eigen::Vector2d{N3_datas(0), N3_datas(1)};
    auto n4_vec = Eigen::Vector2d{N4_datas(0), N4_datas(1)};
    // 计算单元长和宽
    auto length = LenOfNode(n1_vec, n2_vec);
    auto weight = LenOfNode(n4_vec, n3_vec);

    // 获取单元属性
    auto comp_prop = TOOL::GetCompById(model, CompBase::comp_type::prop, _pid);
    auto base_prop = boost::dynamic_pointer_cast<PropertyBase>(comp_prop);
    if (!base_prop) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>属性基类获取失败");
    }
    auto prop_datas = base_prop->GetPropDate();
    auto mid1 = boost::any_cast<int>(prop_datas.at("_mid1"));
    auto t = boost::any_cast<double>(prop_datas.at("_t"));
    auto mid2 = boost::any_cast<int>(prop_datas.at("_mid2"));
    auto inertia_ratio_of_bm =
        boost::any_cast<double>(prop_datas.at("_inertia_ratio_of_bm"));

    // 获取材料参数
    // mid1
    auto comp_mat1 = TOOL::GetCompById(model, CompBase::comp_type::mat, mid1);
    auto base_mat1 = boost::dynamic_pointer_cast<MaterialBase>(comp_mat1);
    if (!base_mat1) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>材料基类获取失败");
    }
    auto mat_data1 = base_mat1->GetMatDate();
    auto mat_info1 = TOOL::DealENuG(mat_data1);
    if (mat_info1.empty()) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Kf::GenerateK)>>>材料获取失败");
    }
    double E1 = mat_info1.at(0);
    double NU1 = mat_info1.at(1);
    double G1 = mat_info1.at(2);
    // mid2
    double E2 = 0.0;
    double NU2 = 0.0;
    double G2 = 0.0;
    if (mid1 == mid2) {
      E2 = E1;
      G2 = G1;
      NU2 = NU1;
    } else {
      auto comp_mat2 = TOOL::GetCompById(model, CompBase::comp_type::mat, mid2);
      auto base_mat2 = boost::dynamic_pointer_cast<MaterialBase>(comp_mat2);
      if (!base_mat2) {
        throw std::runtime_error(
            "[ERROR]:func(Cquad4Kf::GenerateK)>>>mat2材料基类获取失败");
      }
      auto mat_data2 = base_mat2->GetMatDate();
      auto mat_info2 = TOOL::DealENuG(mat_data2);
      if (mat_info2.empty()) {
        throw std::runtime_error(
            "[ERROR]:func(Cquad4Kf::GenerateK)>>>mat2材料获取失败");
      }
      double E2 = mat_info2.at(0);
      double NU2 = mat_info2.at(1);
      double G2 = mat_info2.at(2);
    }

    // =============== 平面应力行为 ===============
    // 平面物理矩阵
    Eigen::MatrixXd D_plane = Eigen::MatrixXd::Zero(3, 3);
    D_plane(0, 0) = E1 / (1 - NU1 * NU1);
    D_plane(0, 1) = (NU1 * E1) / (1 - NU1 * NU1);
    D_plane(1, 0) = (NU1 * E1) / (1 - NU1 * NU1);
    D_plane(1, 1) = E1 / (1 - NU1 * NU1);
    D_plane(2, 2) = G1;

    // 位移-应变矩阵

    // 平面应力刚度矩阵

    // =============== 板弯行为 ===============
    // 板横向弯曲刚度矩阵
    Eigen::MatrixXd k_bend = Eigen::MatrixXd::Zero(12, 12);

    std::array<int, 4> arr_epsilon{0, 1, 1, 0};
    std::array<int, 4> arr_eta{0, 0, 1, 1};
    double H =
        E2 * std::pow(t, 3) / (720 * length * weight * (1 - std::pow(NU2, 2)));
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        double epsilon0 = arr_epsilon.at(i) * arr_epsilon.at(j);
        double eta0 = arr_eta.at(i) * arr_eta.at(j);
        Eigen::Matrix3d k_ij = Eigen::Matrix3d::Zero();
        k_ij(0, 0) =
            3 * H *
            (15 * (std::pow(weight, 2) * epsilon0 / std::pow(length, 2) +
                   std::pow(length, 2) * epsilon0 / std::pow(weight, 2)) +
             epsilon0 * eta0 *
                 (14 - 4 * NU2 +
                  5 * (std::pow(weight, 2) / std::pow(length, 2) +
                       std::pow(length, 2) / std::pow(weight, 2))));

        k_ij(0, 1) =
            -3 * H * weight *
            (epsilon0 * arr_eta.at(i) *
                 (2 + 3 * NU2 + 5 * std::pow(length, 2) / std::pow(weight, 2)) +
             15 * arr_eta.at(i) * std::pow(length, 2) / std::pow(weight, 2) +
             5 * NU2 * epsilon0 * arr_eta.at(j));

        k_ij(0, 2) =
            3 * H * length *
            (arr_epsilon.at(i) * eta0 *
                 (2 + 3 * NU2 + 5 * std::pow(weight, 2) / std::pow(length, 2)) +
             15 * arr_epsilon.at(i) * std::pow(weight, 2) /
                 std::pow(length, 2) +
             5 * NU2 * arr_epsilon.at(j) * eta0);

        k_ij(1, 0) =
            -3 * H * weight *
            (epsilon0 * arr_eta.at(j) *
                 (2 + 3 * NU2 + 5 * std::pow(length, 2) / std::pow(weight, 2)) +
             15 * arr_eta.at(j) * std::pow(length, 2) / std::pow(weight, 2) +
             5 * NU2 * arr_eta.at(i) * epsilon0);

        k_ij(1, 1) = H * std::pow(weight, 2) *
                     (2 * (1 - NU2) * epsilon0 * (3 + 5 * eta0) +
                      5 * std::pow(length, 2) * (3 + epsilon0) * (3 + eta0) /
                          std::pow(weight, 2));

        k_ij(1, 2) = -15 * H * NU2 * length * weight *
                     (arr_epsilon.at(i) + arr_epsilon.at(j)) *
                     (arr_eta.at(i) + arr_eta.at(j));

        k_ij(2, 0) =
            3 * H * length *
            (arr_epsilon.at(j) * eta0 *
                 (2 + 3 * NU2 + 5 * std::pow(weight, 2) / std::pow(length, 2)) +
             15 * arr_epsilon.at(j) * std::pow(weight, 2) /
                 std::pow(length, 2) +
             5 * NU2 * arr_epsilon.at(j) * eta0);

        k_ij(2, 1) = -15 * H * NU2 * length * weight *
                     (arr_epsilon.at(i) + arr_epsilon.at(j)) *
                     (arr_eta.at(i) + arr_eta.at(j));

        k_ij(2, 2) = H * std::pow(length, 2) *
                     (2 * (1 - NU2) * eta0 * (3 + 5 * epsilon0) +
                      5 * (3 + epsilon0) * (3 + eta0) * std::pow(weight, 2) /
                          std::pow(length, 2));

        k_bend.block<4, 4>(i, j) = k_ij;
      }
    }

    // 设置总刚度矩阵

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::MatrixXd Cquad4Kf::GetGlobalK(const MODEL::Model &model) {
  // 获取全局坐标系
  auto global_coord =
      boost::make_shared<COMPONENT::GlobalCoord>()->GetGeneralCoord();
  // 构建局部坐标系
  auto coord_x = TOOL::NodesToCoord(model, _G1, _G2)->_vec1;
  auto coord_z = TOOL::NodesToCoord(model, _G1, _G3)->_vec1;
  auto coord_y = coord_x.cross(coord_z);
  auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
  auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
  if (!N1) {
    throw std::runtime_error(
        "[ERROR]:func(Cquad4Kf::GetGlobalK)>>>局部坐标系原点获取失败");
  }
  auto N1_datas = N1->get_location();
  GeneralCoord loc_coord;
  loc_coord._coord_origin = N1_datas;
  loc_coord._dim_type = GeneralCoord::gen_coord_type::dim3;
  loc_coord._vec1 = coord_x.normalized();
  loc_coord._vec2 = coord_y.normalized();
  loc_coord._vec3 = coord_z.normalized();

  // 生成坐标变换矩阵
  auto trans_matrix_block = TOOL::TransCoordToCoord(global_coord, loc_coord);
  Eigen::MatrixXd trans_matrix = Eigen::MatrixXd::Zero(9, 9);
  trans_matrix.block<3, 3>(0, 0) = trans_matrix_block;
  trans_matrix.block<3, 3>(3, 3) = trans_matrix_block;
  trans_matrix.block<3, 3>(6, 6) = trans_matrix_block;
  // 生成全局坐标系下的单元刚度矩阵
  // 全局坐标系下单元刚度矩阵
  Eigen::MatrixXd global_k;
  global_k = trans_matrix * _loc_k * trans_matrix.transpose();
  return global_k;
}
} // namespace COMPONENT