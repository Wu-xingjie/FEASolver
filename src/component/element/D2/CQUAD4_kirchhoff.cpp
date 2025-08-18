#include "CQUAD4_kirchhoff.h"

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

Cquad4Kf::Cquad4Kf() {
  _elem_type = ElemBase::elem_type::cquad4_kf;
  _loc_k = Eigen::MatrixXd::Zero(24, 24);
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
      E2 = mat_info2.at(0);
      NU2 = mat_info2.at(1);
      G2 = mat_info2.at(2);
    }

    // =============== 平面应力行为 ===============
    // 形函数对自然坐标的偏导
    std::string N1_epsilon = "((y-1)/4)";
    std::string N1_eta = "((x-1)/4)";
    std::string N2_epsilon = "((1-y)/4)";
    std::string N2_eta = "(-(1+x)/4)";
    std::string N3_epsilon = "((1+y)/4)";
    std::string N3_eta = "((1+x)/4)";
    std::string N4_epsilon = "(-(1+y)/4)";
    std::string N4_eta = "((1-x)/4)";
    std::string str_length = std::to_string(length);
    std::string str_weight = std::to_string(weight);
    std::string str_nu1 = std::to_string(NU1);

    Eigen::MatrixXd k_plane = Eigen::MatrixXd::Zero(8, 8);

    std::string exp_k00 = "(" + N1_epsilon + "/" + str_length + ")^2+(2-" +
                          str_nu1 + ")*(" + N1_eta + ")^2/(2*" + str_weight +
                          "^2)";
    auto k00 = TOOL::GaussIntegral(exp_k00, 4, 2);
    k_plane(0, 0) = *k00;

    std::string exp_k01 = "(" + str_nu1 + "*" + N1_epsilon + "*" + N1_eta +
                          "/(" + str_weight + "*" + str_length + "))+((2-" +
                          str_nu1 + ")*" + N1_epsilon + "*" + N1_eta + ")/(2*" +
                          str_length + "*" + str_weight + ")";
    auto k01 = TOOL::GaussIntegral(exp_k01, 4, 2);
    k_plane(1, 0) = *k01;
    k_plane(0, 1) = *k01;

    auto exp_k02 = N1_epsilon + "*" + N2_epsilon + "/(" + str_length +
                   "^2)+(2-" + str_nu1 + ")*" + N1_eta + "*" + N2_eta + "/(2*" +
                   str_weight + "^2)";
    auto k02 = TOOL::GaussIntegral(exp_k02, 4, 2);
    k_plane(2, 0) = *k02;
    k_plane(0, 2) = *k02;

    auto exp_k03 = str_nu1 + "*" + N1_epsilon + "*" + N2_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N1_eta + "*" + N2_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k03 = TOOL::GaussIntegral(exp_k03, 4, 2);
    k_plane(3, 0) = *k03;
    k_plane(0, 3) = *k03;

    auto exp_k04 = N1_epsilon + "*" + N3_epsilon + "/(" + str_length +
                   "^2)+(2-" + str_nu1 + ")*" + N1_eta + "*" + N3_epsilon +
                   "/(2*" + str_weight + "^2)";
    auto k04 = TOOL::GaussIntegral(exp_k04, 4, 2);
    k_plane(4, 0) = *k04;
    k_plane(0, 4) = *k04;

    auto exp_k05 = str_nu1 + "*" + N1_epsilon + "*" + N3_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N1_eta + "*" + N3_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k05 = TOOL::GaussIntegral(exp_k05, 4, 2);
    k_plane(5, 0) = *k05;
    k_plane(0, 5) = *k05;

    auto exp_k06 = N1_epsilon + "*" + N4_epsilon + "/(" + str_length +
                   "^2)+(2-" + str_nu1 + ")*" + N1_eta + "*" + N4_eta + "/(2*" +
                   str_weight + "^2)";
    auto k06 = TOOL::GaussIntegral(exp_k06, 4, 2);
    k_plane(6, 0) = *k06;
    k_plane(0, 6) = *k06;

    auto exp_k07 = str_nu1 + "*" + N1_epsilon + "*" + N4_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N1_eta + "*" + N4_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k07 = TOOL::GaussIntegral(exp_k07, 4, 2);
    k_plane(7, 0) = *k07;
    k_plane(0, 7) = *k07;

    std::string exp_k11 = "(" + N1_eta + "/" + str_weight + ")^2+(2-" +
                          str_nu1 + ")*(" + N1_epsilon + ")^2/(2*" +
                          str_length + "^2)";
    auto k11 = TOOL::GaussIntegral(exp_k11, 4, 2);
    k_plane(1, 1) = *k11;

    auto exp_k12 = str_nu1 + "*" + N1_eta + "*" + N2_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N1_epsilon + "*" + N2_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k12 = TOOL::GaussIntegral(exp_k12, 4, 2);
    k_plane(1, 2) = *k12;
    k_plane(2, 1) = *k12;

    auto exp_k13 = N1_eta + "*" + N2_eta + "/(" + str_weight + "^2)+(2-" +
                   str_nu1 + ")*" + N1_epsilon + "*" + N2_epsilon + "/(2*" +
                   str_length + "^2)";
    auto k13 = TOOL::GaussIntegral(exp_k13, 4, 2);
    k_plane(1, 3) = *k13;
    k_plane(3, 1) = *k13;

    auto exp_k14 = str_nu1 + "*" + N1_eta + "*" + N3_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N1_epsilon + "*" + N3_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k14 = TOOL::GaussIntegral(exp_k14, 4, 2);
    k_plane(1, 4) = *k14;
    k_plane(4, 1) = *k14;

    auto exp_k15 = N1_eta + "*" + N3_eta + "/(" + str_weight + "^2)+(2-" +
                   str_nu1 + ")*" + N1_epsilon + "*" + N3_epsilon + "/(2*" +
                   str_length + "^2)";
    auto k15 = TOOL::GaussIntegral(exp_k15, 4, 2);
    k_plane(1, 5) = *k15;
    k_plane(5, 1) = *k15;

    auto exp_k16 = str_nu1 + "*" + N1_eta + "*" + N4_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N1_epsilon + "*" + N4_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k16 = TOOL::GaussIntegral(exp_k16, 4, 2);
    k_plane(1, 6) = *k16;
    k_plane(6, 1) = *k16;

    auto exp_k17 = N1_eta + "*" + N4_eta + "/(" + str_weight + "^2)+(2-" +
                   str_nu1 + ")*" + N1_epsilon + "*" + N4_epsilon + "/(2*" +
                   str_length + "^2)";
    auto k17 = TOOL::GaussIntegral(exp_k17, 4, 2);
    k_plane(1, 7) = *k17;
    k_plane(7, 1) = *k17;

    std::string exp_k22 = "(" + N2_epsilon + "/" + str_length + ")^2+(2-" +
                          str_nu1 + ")*(" + N2_eta + ")^2/(2*" + str_weight +
                          "^2)";
    auto k22 = TOOL::GaussIntegral(exp_k22, 4, 2);
    k_plane(2, 2) = *k22;

    auto exp_k23 = str_nu1 + "*" + N2_eta + "*" + N2_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N2_epsilon + "*" + N2_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k23 = TOOL::GaussIntegral(exp_k23, 4, 2);
    k_plane(2, 3) = *k23;
    k_plane(3, 2) = *k23;

    auto exp_k24 = N2_epsilon + "*" + N3_epsilon + "/(" + str_length +
                   "^2)+(2-" + str_nu1 + ")*" + N2_eta + "*" + N3_eta + "/(2*" +
                   str_weight + "^2)";
    auto k24 = TOOL::GaussIntegral(exp_k24, 4, 2);
    k_plane(2, 4) = *k24;
    k_plane(4, 2) = *k24;

    auto exp_k25 = str_nu1 + "*" + N2_epsilon + "*" + N3_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N2_eta + "*" + N3_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k25 = TOOL::GaussIntegral(exp_k25, 4, 2);
    k_plane(2, 5) = *k25;
    k_plane(5, 2) = *k25;

    auto exp_k26 = N2_epsilon + "*" + N4_epsilon + "/(" + str_length +
                   "^2)+(2-" + str_nu1 + ")*" + N2_eta + "*" + N4_eta + "/(2*" +
                   str_weight + "^2)";
    auto k26 = TOOL::GaussIntegral(exp_k26, 4, 2);
    k_plane(2, 6) = *k26;
    k_plane(6, 2) = *k26;

    auto exp_k27 = str_nu1 + "*" + N2_epsilon + "*" + N4_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N2_eta + "*" + N4_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k27 = TOOL::GaussIntegral(exp_k27, 4, 2);
    k_plane(2, 5) = *k27;
    k_plane(5, 2) = *k27;

    std::string exp_k33 = "(" + N2_eta + "/" + str_weight + ")^2+(2-" +
                          str_nu1 + ")*(" + N2_epsilon + ")^2/(2*" +
                          str_length + "^2)";
    auto k33 = TOOL::GaussIntegral(exp_k33, 4, 2);
    k_plane(3, 3) = *k33;

    auto exp_k34 = str_nu1 + "*" + N2_eta + "*" + N3_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N2_epsilon + "*" + N3_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k34 = TOOL::GaussIntegral(exp_k34, 4, 2);
    k_plane(3, 4) = *k34;
    k_plane(4, 3) = *k34;

    auto exp_k35 = N2_eta + "*" + N3_eta + "/(" + str_weight + "^2)+(2-" +
                   str_nu1 + ")*" + N2_epsilon + "*" + N3_epsilon + "/(2*" +
                   str_length + "^2)";
    auto k35 = TOOL::GaussIntegral(exp_k35, 4, 2);
    k_plane(3, 5) = *k35;
    k_plane(5, 3) = *k35;

    auto exp_k36 = str_nu1 + "*" + N2_eta + "*" + N4_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N2_epsilon + "*" + N4_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k36 = TOOL::GaussIntegral(exp_k36, 4, 2);
    k_plane(3, 6) = *k36;
    k_plane(6, 3) = *k36;

    auto exp_k37 = N2_eta + "*" + N4_eta + "/(" + str_weight + "^2)+(2-" +
                   str_nu1 + ")*" + N2_epsilon + "*" + N4_epsilon + "/(2*" +
                   str_length + "^2)";
    auto k37 = TOOL::GaussIntegral(exp_k37, 4, 2);
    k_plane(3, 7) = *k37;
    k_plane(7, 3) = *k37;

    std::string exp_k44 = "(" + N3_epsilon + "/" + str_length + ")^2+(2-" +
                          str_nu1 + ")*(" + N3_eta + ")^2/(2*" + str_weight +
                          "^2)";
    auto k44 = TOOL::GaussIntegral(exp_k44, 4, 2);
    k_plane(4, 4) = *k44;

    auto exp_k45 = str_nu1 + "*" + N3_epsilon + "*" + N3_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N3_eta + "*" + N3_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k45 = TOOL::GaussIntegral(exp_k45, 4, 2);
    k_plane(4, 5) = *k45;
    k_plane(5, 4) = *k45;

    auto exp_k46 = N3_epsilon + "*" + N4_epsilon + "/(" + str_length +
                   "^2)+(2-" + str_nu1 + ")*" + N3_eta + "*" + N4_eta + "/(2*" +
                   str_weight + "^2)";
    auto k46 = TOOL::GaussIntegral(exp_k46, 4, 2);
    k_plane(4, 6) = *k46;
    k_plane(6, 4) = *k46;

    auto exp_k47 = str_nu1 + "*" + N3_epsilon + "*" + N4_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N3_eta + "*" + N4_epsilon + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k47 = TOOL::GaussIntegral(exp_k47, 4, 2);
    k_plane(4, 7) = *k47;
    k_plane(7, 4) = *k47;

    std::string exp_k55 = "(" + N3_eta + "/" + str_weight + ")^2+(2-" +
                          str_nu1 + ")*(" + N3_epsilon + ")^2/(2*" +
                          str_length + "^2)";
    auto k55 = TOOL::GaussIntegral(exp_k55, 4, 2);
    k_plane(5, 5) = *k55;

    auto exp_k56 = str_nu1 + "*" + N3_eta + "*" + N4_epsilon + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N3_epsilon + "*" + N4_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k56 = TOOL::GaussIntegral(exp_k56, 4, 2);
    k_plane(5, 6) = *k56;
    k_plane(6, 5) = *k56;

    auto exp_k57 = N3_eta + "*" + N4_eta + "/(" + str_weight + "^2)+(2-" +
                   str_nu1 + ")*" + N3_epsilon + "*" + N4_epsilon + "/(2*" +
                   str_length + "^2)";
    auto k57 = TOOL::GaussIntegral(exp_k57, 4, 2);
    k_plane(5, 7) = *k57;
    k_plane(7, 5) = *k57;

    std::string exp_k66 = "(" + N4_epsilon + "/" + str_length + ")^2+(2-" +
                          str_nu1 + ")*(" + N4_eta + ")^2/(2*" + str_weight +
                          "^2)";
    auto k66 = TOOL::GaussIntegral(exp_k66, 4, 2);
    k_plane(6, 6) = *k66;

    auto exp_k67 = str_nu1 + "*" + N4_epsilon + "*" + N4_eta + "/(" +
                   str_weight + "*" + str_length + ")+(2-" + str_nu1 + ")*" +
                   N4_epsilon + "*" + N4_eta + "/(2*" + str_weight + "*" +
                   str_length + ")";
    auto k67 = TOOL::GaussIntegral(exp_k67, 4, 2);
    k_plane(6, 7) = *k67;
    k_plane(7, 6) = *k67;

    std::string exp_k77 = "(" + N4_eta + "/" + str_weight + ")^2+(" +
                          N4_epsilon + ")^2*(2-" + str_nu1 + ")/(2*" + str_length + "^2)";
    auto k77 = TOOL::GaussIntegral(exp_k77, 4, 2);
    k_plane(7, 7) = *k77;

    // TOOL::DisplayMatrixXd(k_plane);
    double temp = E2 / (1 - std::pow(NU1, 2));
    k_plane = length * weight * t * k_plane * E2 / (1 - std::pow(NU1, 2));

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

        k_bend.block<3, 3>(3 * j, 3 * i) = k_ij;
      }
    }

    // std::cout << "k_bend:" << std::endl;
    // TOOL::DisplayMatrixXd(k_bend);

    // 设置总刚度矩阵
    // 1: 把板弯刚度矩阵和膜刚度矩阵扩容成36*36的全自由度刚度矩阵
    // 1.1:生成所有节点自由度到矩阵索引的映射
    std::map<std::string, int> dof2index;
    std::vector<std::string> vec_dof{"x",       "y",       "z",
                                     "theta_x", "theta_y", "theta_z"};

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 6; j++) {
        std::string str_dof = std::to_string(i + 1) + "_" + vec_dof.at(j);
        dof2index[str_dof] = i * 6 + j;
      }
    }
    // 1.2: 将膜刚度矩阵扩容
    Eigen::MatrixXd plane_k_alldof = Eigen::MatrixXd::Zero(24, 24);
    std::map<int, std::string> plane_k_map{
        {0, "1_x"}, {1, "1_y"}, {2, "2_x"}, {3, "2_y"},
        {4, "3_x"}, {5, "3_y"}, {6, "4_x"}, {7, "4_y"},
    };
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        std::string dof_col = plane_k_map.at(i);
        std::string dof_row = plane_k_map.at(j);
        int idx_col = dof2index.at(dof_col);
        int idx_row = dof2index.at(dof_row);
        plane_k_alldof(idx_col, idx_row) = k_plane(i, j);
      }
    }
    // 1.2: 将板弯刚度矩阵扩容
    Eigen::MatrixXd bend_k_alldof = Eigen::MatrixXd::Zero(24, 24);
    std::map<int, std::string> bend_k_map{
        {0, "1_z"}, {1, "1_theta_x"},  {2, "1_theta_y"},
        {3, "2_z"}, {4, "2_theta_x"},  {5, "2_theta_y"},
        {6, "3_z"}, {7, "3_theta_x"},  {8, "3_theta_y"},
        {9, "4_z"}, {10, "4_theta_x"}, {11, "4_theta_y"},
    };
    for (int i = 0; i < 12; i++) {
      for (int j = 0; j < 12; j++) {
        std::string dof_col = bend_k_map.at(i);
        std::string dof_row = bend_k_map.at(j);
        int idx_col = dof2index.at(dof_col);
        int idx_row = dof2index.at(dof_row);
        bend_k_alldof(idx_col, idx_row) = k_bend(i, j);
      }
    }
    TOOL::DisplayMatrixXd(k_plane, "k_plane");
    TOOL::DisplayMatrixXd(plane_k_alldof, "plane_k_alldof");
    TOOL::DisplayMatrixXd(k_bend, "k_bend");
    TOOL::DisplayMatrixXd(bend_k_alldof, "bend_k_alldof");

    // 2:将扩容后的板弯刚度矩阵和膜刚度矩阵相加；
    _loc_k = plane_k_alldof + bend_k_alldof;
    TOOL::DisplayMatrixXd(_loc_k, "_loc_k");
  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::MatrixXd Cquad4Kf::GetGlobalK(const MODEL::Model &model) {
  // 获取全局坐标系
  auto global_coord =
      boost::make_shared<COMPONENT::GlobalCoord>()->GetGeneralCoord();
  // 构建局部坐标系
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
  // TOOL::DisplayMatrixXd(trans_matrix, "trans_matrix");
  global_k = trans_matrix * _loc_k * trans_matrix.transpose();

  // TOOL::DisplayMatrixXd(global_k, "global_k");
  return global_k;
}
} // namespace COMPONENT