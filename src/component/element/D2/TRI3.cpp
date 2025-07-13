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
        "-" + std::to_string(*L1_y) + "(" + TOOL::AreaFuncExpr(p_2, p_3) + ")";
    auto E01 = TOOL::TriGaussIntegral(expr_E01, {p_1, p_2, p_3}, 3, t);
    SetElemOfMatrixB(V2, 0, 1, '+', E01);

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

} // namespace COMPONENT