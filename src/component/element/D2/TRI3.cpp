#include "TRI3.h"

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

Tri3::Tri3() {
  _elem_type = ElemBase::elem_type::tri3;
  _loc_k = Eigen::MatrixXd::Zero(18, 18);
}

void Tri3::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
  _G3 = boost::any_cast<int>(card.at(5));
}

boost::shared_ptr<double> Tri3::AreaCoordPartialDerivate(
    const Eigen::Vector3d &n1, const Eigen::Vector3d &n2, const char &lab) {
  auto result = boost::make_shared<double>();
  if (lab == 'x') {
    *result = n1(2) - n2(2);
  } else if (lab == 'y') {
    *result = n2(1) - n1(1);
  }
  return result;
}

void Tri3::SetElemOfMatrixB(Eigen::MatrixXd &B, const int &r, const int &c,
                            const char &p,
                            const boost::shared_ptr<double> val) {
  if (!val) {
    throw std::runtime_error(
        "[ERROR]:func(Tri3::SetElemOfMatrixB)>>>传入参数有误!");
  }
  if (p == '+') {
    B(r, c) = *val;
  } else if (p == '-') {
    B(r, c) = *val * -1;
  } else {
    throw std::runtime_error(
        "[ERROR]:func(Tri3::SetElemOfMatrixB)>>>传入参数正负号有误!");
  }
}

void Tri3::GenerateK(const MODEL::Model &model) {
  try {
    // 获取节点
    auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error("[ERROR]:func(Tri3::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error("[ERROR]:func(Tri3::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();
    auto comp_N3 = TOOL::GetCompById(model, CompBase::comp_type::node, _G3);
    auto N3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N3);
    if (!N3) {
      throw std::runtime_error("[ERROR]:func(Tri3::GenerateK)>>>节点3获取失败");
    }
    auto N3_datas = N3->get_location();

    // 计算单元面积
    auto n1_vec = Eigen::Vector2d{N1_datas(0), N1_datas(1)};
    auto n2_vec = Eigen::Vector2d{N2_datas(0), N2_datas(1)};
    auto n3_vec = Eigen::Vector2d{N3_datas(0), N3_datas(1)};
    // 计算三角形面积
    auto area = TOOL::AreaOfThreePoints(n1_vec, n2_vec, n3_vec);
    if (!area) {
      throw std::runtime_error(
          "[ERROR]:func(Tri3::GenerateK)>>>单元面积计算失败");
    }

    // 获取单元属性
    auto comp_prop = TOOL::GetCompById(model, CompBase::comp_type::prop, _pid);
    auto base_prop = boost::dynamic_pointer_cast<PropertyBase>(comp_prop);
    if (!base_prop) {
      throw std::runtime_error(
          "[ERROR]:func(Tri3::GenerateK)>>>属性基类获取失败");
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
          "[ERROR]:func(Tri3::GenerateK)>>>材料基类获取失败");
    }
    auto mat_data1 = base_mat1->GetMatDate();
    auto mat_info1 = TOOL::DealENuG(mat_data1);
    if (mat_info1.empty()) {
      throw std::runtime_error("[ERROR]:func(Tri3::GenerateK)>>>材料获取失败");
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
            "[ERROR]:func(Tri3::GenerateK)>>>mat2材料基类获取失败");
      }
      auto mat_data2 = base_mat2->GetMatDate();
      auto mat_info2 = TOOL::DealENuG(mat_data2);
      if (mat_info2.empty()) {
        throw std::runtime_error(
            "[ERROR]:func(Tri3::GenerateK)>>>mat2材料获取失败");
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
    Eigen::MatrixXd B_plane = Eigen::MatrixXd::Zero(3, 9);
    auto B00 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'x');
    SetElemOfMatrixB(B_plane, 0, 0, '+', B00);

    auto B02 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'x');
    SetElemOfMatrixB(B_plane, 0, 2, '+', B02);

    auto B04 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'x');
    SetElemOfMatrixB(B_plane, 0, 4, '+', B04);

    auto B11 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'y');
    SetElemOfMatrixB(B_plane, 1, 1, '+', B11);

    auto B13 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'y');
    SetElemOfMatrixB(B_plane, 1, 3, '+', B13);

    auto B15 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'y');
    SetElemOfMatrixB(B_plane, 1, 5, '+', B15);

    auto B20 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'y');
    SetElemOfMatrixB(B_plane, 2, 0, '+', B20);

    auto B21 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'x');
    SetElemOfMatrixB(B_plane, 2, 1, '+', B21);

    auto B22 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'y');
    SetElemOfMatrixB(B_plane, 2, 2, '+', B22);

    auto B23 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'x');
    SetElemOfMatrixB(B_plane, 2, 3, '+', B23);

    auto B24 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'y');
    SetElemOfMatrixB(B_plane, 2, 4, '+', B24);

    auto B25 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'x');
    SetElemOfMatrixB(B_plane, 2, 5, '+', B25);

    // 平面应力刚度矩阵
    auto k_plane = B_plane.transpose() * D_plane * B_plane * (*area) * t;

    // =============== 板弯行为 ===============

    // 因为位移-应变矩阵(B)后两行存在x和y的函数，前四行求过偏导数后都是常数矩阵。
    // 而且应变-应力矩阵(D)除了左上角4*4子矩阵和右下角2*2子矩阵以外都是零矩阵
    // 所以，将矩阵B按照前四行和后两行分别处理

    // 物理矩阵
    Eigen::MatrixXd D1_bend = Eigen::MatrixXd::Zero(4, 4);
    D1_bend(0, 0) = 1 / E2;
    D1_bend(1, 1) = 1 / E2;
    D1_bend(2, 2) = 1 / E2;
    D1_bend(1, 2) = -NU2 / E2;
    D1_bend(2, 1) = -NU2 / E2;
    D1_bend(1, 3) = -NU2 / E2;
    D1_bend(3, 1) = -NU2 / E2;
    D1_bend(3, 2) = -NU2 / E2;
    D1_bend(2, 3) = -NU2 / E2;
    D1_bend(4, 4) = 1 / G2;

    auto L1_x = AreaCoordPartialDerivate(N2_datas, N3_datas, 'x');
    auto L1_y = AreaCoordPartialDerivate(N2_datas, N3_datas, 'y');
    auto L2_x = AreaCoordPartialDerivate(N3_datas, N1_datas, 'x');
    auto L2_y = AreaCoordPartialDerivate(N3_datas, N1_datas, 'y');
    auto L3_x = AreaCoordPartialDerivate(N1_datas, N2_datas, 'x');
    auto L3_y = AreaCoordPartialDerivate(N1_datas, N2_datas, 'y');
    Eigen::Vector2d p_1{N1_datas(0), N1_datas(1)};
    Eigen::Vector2d p_2{N2_datas(0), N2_datas(1)};
    Eigen::Vector2d p_3{N3_datas(0), N3_datas(1)};
    if (!L1_x or !L1_y or !L2_x or !L2_y or !L3_x or !L3_y) {
      throw std::runtime_error(
          "[ERROR]:func(Tri3::GenerateK)>>>面积坐标求偏导异常！");
    }

    // 给位移-应变矩阵前四行赋值
    Eigen::MatrixXd B1 = Eigen::MatrixXd::Zero(4, 9);
    auto B02_expr = "z*" + std::to_string(*L1_x);
    auto B02_val = TOOL::TriGaussIntegral(B02_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 0, 2, '+', B02_val);

    auto B05_expr = "z*" + std::to_string(*L2_x);
    auto B05_val = TOOL::TriGaussIntegral(B05_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 0, 5, '+', B05_val);

    auto B08_expr = "z*" + std::to_string(*L3_x);
    auto B08_val = TOOL::TriGaussIntegral(B08_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 0, 8, '+', B08_val);

    auto B11_expr = "-z*" + std::to_string(*L1_y);
    auto B11_val = TOOL::TriGaussIntegral(B11_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 1, 1, '+', B11_val);

    auto B14_expr = "-z*" + std::to_string(*L2_y);
    auto B14_val = TOOL::TriGaussIntegral(B14_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 1, 4, '+', B14_val);

    auto B17_expr = "-z*" + std::to_string(*L3_y);
    auto B17_val = TOOL::TriGaussIntegral(B17_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 1, 7, '+', B17_val);

    auto B31_expr = "-z*" + std::to_string(*L1_x);
    auto B31_val = TOOL::TriGaussIntegral(B31_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 3, 1, '+', B31_val);

    auto B32_expr = "z*" + std::to_string(*L1_y);
    auto B32_val = TOOL::TriGaussIntegral(B32_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 3, 2, '+', B32_val);

    auto B34_expr = "-z*" + std::to_string(*L2_x);
    auto B34_val = TOOL::TriGaussIntegral(B34_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 3, 4, '+', B34_val);

    auto B35_expr = "z*" + std::to_string(*L2_y);
    auto B35_val = TOOL::TriGaussIntegral(B35_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 3, 5, '+', B35_val);

    auto B37_expr = "-z*" + std::to_string(*L3_x);
    auto B37_val = TOOL::TriGaussIntegral(B37_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 3, 7, '+', B37_val);

    auto B38_expr = "z*" + std::to_string(*L2_y);
    auto B38_val = TOOL::TriGaussIntegral(B38_expr, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(B1, 3, 8, '+', B38_val);

    auto V1 = B1.transpose() * D1_bend * B1;

    // 给位移-应变矩阵后两行对应的应变能矩阵赋值
    Eigen::MatrixXd V2 = Eigen::MatrixXd::Zero(9, 9);

    auto E00 = (std::pow(*L1_x, 2) + std::pow(*L1_y, 2)) * t * (*area);
    SetElemOfMatrixB(V2, 0, 0, '+', boost::make_shared<double>(E00));

    auto expr_E01 =
        "-" + std::to_string(*L1_y) + "*(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E01 = TOOL::TriGaussIntegral(expr_E01, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 1, '+', E01);
    SetElemOfMatrixB(V2, 1, 0, '+', E01);

    auto expr_E02 =
        std::to_string(*L1_x) + "*(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E02 = TOOL::TriGaussIntegral(expr_E02, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 2, '+', E02);
    SetElemOfMatrixB(V2, 2, 0, '+', E02);

    auto E03 = (*L1_x) * (*L2_x) + (*L1_y) * (*L2_y);
    SetElemOfMatrixB(V2, 0, 3, '+', boost::make_shared<double>(E03));
    SetElemOfMatrixB(V2, 3, 0, '+', boost::make_shared<double>(E03));

    auto expr_E04 =
        "-" + std::to_string(*L1_y) + "*(" + TOOL::AreaFuncExpr(p_3, p_1) + ")";
    auto E04 = TOOL::TriGaussIntegral(expr_E04, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 4, '+', E04);
    SetElemOfMatrixB(V2, 4, 0, '+', E04);

    auto expr_E05 =
        std::to_string(*L1_x) + "*(" + TOOL::AreaFuncExpr(p_3, p_1) + ")";
    auto E05 = TOOL::TriGaussIntegral(expr_E05, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 5, '+', E05);
    SetElemOfMatrixB(V2, 5, 0, '+', E05);

    auto E06 = (*L1_x) * (*L3_x) + (*L1_y) * (*L3_y);
    SetElemOfMatrixB(V2, 0, 6, '+', boost::make_shared<double>(E06));
    SetElemOfMatrixB(V2, 6, 0, '+', boost::make_shared<double>(E06));

    auto expr_E07 =
        "-" + std::to_string(*L1_y) + "*(" + TOOL::AreaFuncExpr(p_1, p_2) + ")";
    auto E07 = TOOL::TriGaussIntegral(expr_E07, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 7, '+', E07);
    SetElemOfMatrixB(V2, 7, 0, '+', E07);

    auto expr_E08 =
        std::to_string(*L1_x) + "*(" + TOOL::AreaFuncExpr(p_1, p_2) + ")";
    auto E08 = TOOL::TriGaussIntegral(expr_E08, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 8, '+', E08);
    SetElemOfMatrixB(V2, 8, 0, '+', E08);

    auto expr_E11 = TOOL::AreaFuncExpr(p_2, p_3) + "^2";
    auto E11 = TOOL::TriGaussIntegral(expr_E11, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 1, 1, '+', E11);

    auto expr_E13 =
        "-" + std::to_string(*L2_y) + "*(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E13 = TOOL::TriGaussIntegral(expr_E13, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 1, 3, '+', E13);
    SetElemOfMatrixB(V2, 3, 1, '+', E13);

    auto expr_E14 =
        TOOL::AreaFuncExpr(p_2, p_3) + "*" + TOOL::AreaFuncExpr(p_3, p_1);
    auto E14 = TOOL::TriGaussIntegral(expr_E14, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 1, 4, '+', E14);
    SetElemOfMatrixB(V2, 4, 1, '+', E14);

    auto expr_E16 =
        "-" + std::to_string(*L3_y) + "*(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E16 = TOOL::TriGaussIntegral(expr_E16, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 1, 6, '+', E16);
    SetElemOfMatrixB(V2, 6, 1, '+', E16);

    auto expr_E17 =
        TOOL::AreaFuncExpr(p_2, p_3) + "*" + TOOL::AreaFuncExpr(p_1, p_2);
    auto E17 = TOOL::TriGaussIntegral(expr_E17, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 1, 7, '+', E17);
    SetElemOfMatrixB(V2, 7, 1, '+', E17);

    auto expr_E22 = TOOL::AreaFuncExpr(p_2, p_3) + "^2";
    auto E22 = TOOL::TriGaussIntegral(expr_E22, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 2, 2, '+', E22);

    auto expr_E23 =
        std::to_string(*L2_x) + "*(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E23 = TOOL::TriGaussIntegral(expr_E23, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 2, 3, '+', E23);
    SetElemOfMatrixB(V2, 3, 2, '+', E23);

    auto expr_E25 =
        TOOL::AreaFuncExpr(p_2, p_3) + "*" + TOOL::AreaFuncExpr(p_3, p_1);
    auto E25 = TOOL::TriGaussIntegral(expr_E25, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 2, 5, '+', E25);
    SetElemOfMatrixB(V2, 5, 2, '+', E25);

    auto expr_E26 =
        std::to_string(*L3_x) + "*(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E26 = TOOL::TriGaussIntegral(expr_E26, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 2, 6, '+', E26);
    SetElemOfMatrixB(V2, 6, 2, '+', E26);

    auto expr_E28 =
        TOOL::AreaFuncExpr(p_2, p_3) + "*" + TOOL::AreaFuncExpr(p_1, p_2);
    auto E28 = TOOL::TriGaussIntegral(expr_E28, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 2, 8, '+', E28);
    SetElemOfMatrixB(V2, 8, 2, '+', E28);

    auto E33 = (std::pow(*L2_x, 2) + std::pow(*L2_y, 2)) * t * (*area);
    SetElemOfMatrixB(V2, 0, 0, '+', boost::make_shared<double>(E33));

    auto expr_E34 =
        "-" + std::to_string(*L2_y) + "*(" + TOOL::AreaFuncExpr(p_3, p_1) + ")";
    auto E34 = TOOL::TriGaussIntegral(expr_E34, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 3, 4, '+', E34);
    SetElemOfMatrixB(V2, 4, 3, '+', E34);

    auto expr_E35 =
        std::to_string(*L2_x) + "*(" + TOOL::AreaFuncExpr(p_3, p_1) + ")";
    auto E35 = TOOL::TriGaussIntegral(expr_E35, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 3, 5, '+', E35);
    SetElemOfMatrixB(V2, 5, 3, '+', E35);

    auto E36 = (*L2_x) * (*L3_x) + (*L2_y) * (*L3_y);
    SetElemOfMatrixB(V2, 3, 6, '+', boost::make_shared<double>(E36));
    SetElemOfMatrixB(V2, 6, 3, '+', boost::make_shared<double>(E36));

    auto expr_E37 =
        "-" + std::to_string(*L2_y) + "*(" + TOOL::AreaFuncExpr(p_1, p_2) + ")";
    auto E37 = TOOL::TriGaussIntegral(expr_E37, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 3, 7, '+', E37);
    SetElemOfMatrixB(V2, 7, 3, '+', E37);

    auto expr_E38 =
        std::to_string(*L2_x) + "*(" + TOOL::AreaFuncExpr(p_1, p_2) + ")";
    auto E38 = TOOL::TriGaussIntegral(expr_E38, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 3, 8, '+', E38);
    SetElemOfMatrixB(V2, 8, 3, '+', E38);

    auto expr_E44 = TOOL::AreaFuncExpr(p_3, p_1) + "^2";
    auto E44 = TOOL::TriGaussIntegral(expr_E44, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 4, 4, '+', E44);

    auto expr_E46 =
        "-" + std::to_string(*L3_y) + "*(" + TOOL::AreaFuncExpr(p_3, p_1) + ")";
    auto E46 = TOOL::TriGaussIntegral(expr_E46, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 4, 6, '+', E46);
    SetElemOfMatrixB(V2, 6, 4, '+', E46);

    auto expr_E47 =
        TOOL::AreaFuncExpr(p_3, p_1) + "*" + TOOL::AreaFuncExpr(p_1, p_2);
    auto E47 = TOOL::TriGaussIntegral(expr_E47, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 4, 7, '+', E47);
    SetElemOfMatrixB(V2, 7, 4, '+', E47);

    auto expr_E55 = TOOL::AreaFuncExpr(p_3, p_1) + "^2";
    auto E55 = TOOL::TriGaussIntegral(expr_E55, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 5, 5, '+', E55);

    auto expr_E56 =
        std::to_string(*L3_x) + "*(" + TOOL::AreaFuncExpr(p_3, p_1) + ")";
    auto E56 = TOOL::TriGaussIntegral(expr_E56, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 5, 6, '+', E56);
    SetElemOfMatrixB(V2, 6, 5, '+', E56);

    auto expr_E58 =
        TOOL::AreaFuncExpr(p_3, p_1) + "*" + TOOL::AreaFuncExpr(p_1, p_2);
    auto E58 = TOOL::TriGaussIntegral(expr_E58, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 5, 8, '+', E58);
    SetElemOfMatrixB(V2, 8, 5, '+', E58);

    auto E66 = (std::pow(*L3_x, 2) + std::pow(*L3_y, 2)) * t * (*area);
    SetElemOfMatrixB(V2, 6, 6, '+', boost::make_shared<double>(E66));

    auto expr_E67 =
        "-" + std::to_string(*L3_y) + "*(" + TOOL::AreaFuncExpr(p_1, p_2) + ")";
    auto E67 = TOOL::TriGaussIntegral(expr_E67, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 6, 7, '+', E67);
    SetElemOfMatrixB(V2, 7, 6, '+', E67);

    auto expr_E68 =
        std::to_string(*L3_x) + "*(" + TOOL::AreaFuncExpr(p_1, p_2) + ")";
    auto E68 = TOOL::TriGaussIntegral(expr_E68, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 6, 8, '+', E68);
    SetElemOfMatrixB(V2, 8, 6, '+', E68);

    auto expr_E77 = TOOL::AreaFuncExpr(p_1, p_2) + "^2";
    auto E77 = TOOL::TriGaussIntegral(expr_E77, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 7, 7, '+', E77);

    SetElemOfMatrixB(V2, 8, 8, '+', E77);

    // 板横向弯曲刚度矩阵
    Eigen::MatrixXd V_bending = V1 + V2 / G2;

    // 设置总刚度矩阵
    _loc_k = k_plane + V_bending;

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::MatrixXd Tri3::GetGlobalK(const MODEL::Model &model) {
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
        "[ERROR]:func(Tri3::GetGlobalK)>>>局部坐标系原点获取失败");
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
}  // namespace COMPONENT