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

boost::shared_ptr<double> Tri3::AreaCoordPartialDerivate(const vec_3 &n1,
                                                         const vec_3 &n2,
                                                         const char &lab) {
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
    auto area = TOOL::AreaOfThreePoints({N1_datas(0), N1_datas(1)},
                                        {N2_datas(0), N2_datas(1)},
                                        {N3_datas(0), N3_datas(1)});
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
            "[ERROR]:func(Tri3::GenerateK)>>>材料基类获取失败");
      }
      auto mat_data2 = base_mat2->GetMatDate();
      auto mat_info2 = TOOL::DealENuG(mat_data2);
      if (mat_info2.empty()) {
        throw std::runtime_error(
            "[ERROR]:func(Tri3::GenerateK)>>>材料获取失败");
      }
      double E2 = mat_info2.at(0);
      double NU2 = mat_info2.at(1);
      double G2 = mat_info2.at(2);
    }

    // =============== 板弯行为 ===============

    // 因为位移-应变矩阵(B)后两行存在x和y的函数，前四行求过偏导数后都是常数矩阵。
    // 而且应变-应力矩阵(D)除了左上角4*4子矩阵和右下角2*2子矩阵以外都是零矩阵
    // 所以，将矩阵B按照前四行和后两行分别处理

    // 物理矩阵
    Eigen::MatrixXd D1 = Eigen::MatrixXd::Zero(4, 4);
    D1(0, 0) = 1 / E2;
    D1(1, 1) = 1 / E2;
    D1(2, 2) = 1 / E2;
    D1(1, 2) = -NU2 / E2;
    D1(2, 1) = -NU2 / E2;
    D1(1, 3) = -NU2 / E2;
    D1(3, 1) = -NU2 / E2;
    D1(3, 2) = -NU2 / E2;
    D1(2, 3) = -NU2 / E2;
    D1(4, 4) = 1 / G2;

    Eigen::MatrixXd D2 = Eigen::MatrixXd::Zero(2, 2);
    D2(5, 5) = 1 / G2;
    D2(6, 6) = 1 / G2;

    // 给位移-应变矩阵前四行赋值
    Eigen::MatrixXd B1 = Eigen::MatrixXd::Zero(4, 9);
    auto B02 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'x');
    SetElemOfMatrixB(B1, 0, 2, '+', B02);
    auto B05 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'x');
    SetElemOfMatrixB(B1, 0, 5, '+', B05);
    auto B08 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'x');
    SetElemOfMatrixB(B1, 0, 8, '+', B08);
    auto B11 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'y');
    SetElemOfMatrixB(B1, 1, 1, '-', B11);
    auto B14 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'y');
    SetElemOfMatrixB(B1, 1, 4, '-', B14);
    auto B17 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'y');
    SetElemOfMatrixB(B1, 1, 7, '-', B17);
    auto B31 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'x');
    SetElemOfMatrixB(B1, 3, 1, '-', B31);
    auto B32 = AreaCoordPartialDerivate(N2_datas, N3_datas, 'y');
    SetElemOfMatrixB(B1, 3, 2, '+', B32);
    auto B34 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'x');
    SetElemOfMatrixB(B1, 3, 4, '-', B34);
    auto B35 = AreaCoordPartialDerivate(N3_datas, N1_datas, 'y');
    SetElemOfMatrixB(B1, 3, 5, '+', B35);
    auto B37 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'x');
    SetElemOfMatrixB(B1, 3, 7, '-', B37);
    auto B38 = AreaCoordPartialDerivate(N1_datas, N2_datas, 'y');
    SetElemOfMatrixB(B1, 3, 8, '+', B38);

    auto V1 = B1.inverse() * D1 * B1;

    // 给位移-应变矩阵后两行对应的应变能矩阵赋值
    Eigen::MatrixXd V2 = Eigen::MatrixXd::Zero(9, 9);
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

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

}  // namespace COMPONENT