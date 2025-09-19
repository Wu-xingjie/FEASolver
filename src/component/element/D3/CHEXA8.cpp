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

void Chexa8::GenerateK(const MODEL::Model &model) {

  try {
    // 获取节点
    auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();

    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();

    auto comp_N3 = TOOL::GetCompById(model, CompBase::comp_type::node, _G3);
    auto N3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N3);
    if (!N3) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点3获取失败");
    }
    auto N3_datas = N3->get_location();

    auto comp_N4 = TOOL::GetCompById(model, CompBase::comp_type::node, _G4);
    auto N4 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N4);
    if (!N4) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点4获取失败");
    }
    auto N4_datas = N4->get_location();

    auto comp_N5 = TOOL::GetCompById(model, CompBase::comp_type::node, _G5);
    auto N5 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N5);
    if (!N5) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点5获取失败");
    }
    auto N5_datas = N5->get_location();

    auto comp_N6 = TOOL::GetCompById(model, CompBase::comp_type::node, _G6);
    auto N6 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N6);
    if (!N6) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点6获取失败");
    }
    auto N6_datas = N6->get_location();

    auto comp_N7 = TOOL::GetCompById(model, CompBase::comp_type::node, _G7);
    auto N7 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N7);
    if (!N7) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点7获取失败");
    }
    auto N7_datas = N7->get_location();

    auto comp_N8 = TOOL::GetCompById(model, CompBase::comp_type::node, _G8);
    auto N8 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N8);
    if (!N8) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>节点8获取失败");
    }
    auto N8_datas = N8->get_location();

    std::array<double, 8> x_arr{N1_datas(0), N2_datas(0), N3_datas(0),
                                N4_datas(0), N5_datas(0), N6_datas(0),
                                N7_datas(0), N8_datas(0)};
    std::array<double, 8> y_arr{N1_datas(1), N2_datas(1), N3_datas(1),
                                N4_datas(1), N5_datas(1), N6_datas(1),
                                N7_datas(1), N8_datas(1)};
    std::array<double, 8> z_arr{N1_datas(2), N2_datas(2), N3_datas(2),
                                N4_datas(2), N5_datas(2), N6_datas(2),
                                N7_datas(2), N8_datas(2)};
    std::array<std::array<double, 8>, 3> point_arr{x_arr, y_arr, z_arr};

    // 等参单元在自然坐标系下单元的边长为2
    double length = 1;
    double weight = 1;
    double height = 1;

    // 获取单元属性
    auto comp_prop = TOOL::GetCompById(model, CompBase::comp_type::prop, _pid);
    auto base_prop = boost::dynamic_pointer_cast<PropertyBase>(comp_prop);
    if (!base_prop) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>属性基类获取失败");
    }
    auto prop_datas = base_prop->GetPropDate();
    auto mid = boost::any_cast<int>(prop_datas.at("_mid"));

    // 获取材料参数
    auto comp_mat = TOOL::GetCompById(model, CompBase::comp_type::mat, mid);
    auto base_mat = boost::dynamic_pointer_cast<MaterialBase>(comp_mat);
    if (!base_mat) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>材料基类获取失败");
    }
    auto mat_data = base_mat->GetMatDate();
    auto mat_info = TOOL::DealENuG(mat_data);
    if (mat_info.empty()) {
      throw std::runtime_error(
          "[ERROR]:func(Chexa8::GenerateK)>>>材料获取失败");
    }
    double E = mat_info.at(0);
    double NU = mat_info.at(1);

    // 应力-应变矩阵
    Eigen::MatrixXd D = Eigen::MatrixXd::Zero(6, 6);
    D(0, 0) = 1;
    D(1, 1) = 1;
    D(2, 2) = 1;
    D(0, 1) = NU / (1 - NU);
    D(1, 0) = NU / (1 - NU);
    D(0, 2) = NU / (1 - NU);
    D(2, 0) = NU / (1 - NU);
    D(1, 2) = NU / (1 - NU);
    D(2, 1) = NU / (1 - NU);
    D(3, 3) = (1 - 2 * NU) / (2 * (1 - NU));
    D(4, 4) = (1 - 2 * NU) / (2 * (1 - NU));
    D(5, 5) = (1 - 2 * NU) / (2 * (1 - NU));
    D = E * (1 - NU) * D / ((1 + NU) * (1 - 2 * NU));

    // 以字符串的形式表示出形函数对自然坐标的偏导
    std::string str_N1_epsilon = "(1-y)*(1-z)/8";
    std::string str_N1_eta = "-(1+x)*(1-z)/8";
    std::string str_N1_gama = "-(1+x)*(1-y)/8";
    std::string str_N2_epsilon = "(1+y)*(1-z)/8";
    std::string str_N2_eta = "(1+x)*(1-z)/8";
    std::string str_N2_gama = "-(1+x)*(1+y)/8";
    std::string str_N3_epsilon = "-(1+y)*(1-z)/8";
    std::string str_N3_eta = "(1-x)*(1-z)/8";
    std::string str_N3_gama = "-(1-x)*(1+y)/8";
    std::string str_N4_epsilon = "-(1-y)*(1-z)/8";
    std::string str_N4_eta = "-(1-x)*(1-z)/8";
    std::string str_N4_gama = "-(1-x)*(1-y)/8";
    std::string str_N5_epsilon = "(1-y)*(1+z)/8";
    std::string str_N5_eta = "-(1+x)*(1+z)/8";
    std::string str_N5_gama = "(1+x)*(1-y)/8";
    std::string str_N6_epsilon = "(1+y)*(1+z)/8";
    std::string str_N6_eta = "(1+x)*(1+z)/8";
    std::string str_N6_gama = "(1+x)*(1+y)/8";
    std::string str_N7_epsilon = "-(1+y)*(1+z)/8";
    std::string str_N7_eta = "(1-x)*(1+z)/8";
    std::string str_N7_gama = "(1-x)*(1+y)/8";
    std::string str_N8_epsilon = "-(1-y)*(1+z)/8";
    std::string str_N8_eta = "-(1-x)*(1+z)/8";
    std::string str_N8_gama = "(1-x)*(1-y)/8";

    std::array<std::string, 8> Ni_epsilon{
        str_N1_epsilon, str_N2_epsilon, str_N3_epsilon, str_N4_epsilon,
        str_N5_epsilon, str_N6_epsilon, str_N7_epsilon, str_N8_epsilon};

    std::array<std::string, 8> Ni_eta{str_N1_eta, str_N2_eta, str_N3_eta,
                                      str_N4_eta, str_N5_eta, str_N6_eta,
                                      str_N7_eta, str_N8_eta};

    std::array<std::string, 8> Ni_gama{str_N1_gama, str_N2_gama, str_N3_gama,
                                       str_N4_gama, str_N5_gama, str_N6_gama,
                                       str_N7_gama, str_N8_gama};

    std::array<std::array<std::string, 8>, 3> arr_Ni_partial = {
        Ni_epsilon, Ni_eta, Ni_gama};

    // 拿到高斯积分的采样点和积分权值
    auto gauss_sample = TOOL::GetGaussSampPoint(3);
    auto gauss_weight = TOOL::GetGaussWeightVal(3);

    // 获取单元刚度矩阵
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
          //  计算jacob矩阵行列式
          Eigen::Matrix3d jacob = Eigen::Matrix3d::Zero();
          for (int kk = 0; kk < 3; kk++) {
            for (int ii = 0; ii < 3; ii++) {
              std::string str_jacob_ki = "";
              for (int jj = 0; jj < 3; jj++) {
                str_jacob_ki += "+(" + arr_Ni_partial.at(kk).at(jj) + ")*" +
                                std::to_string(point_arr.at(ii).at(jj));
              }
              jacob(kk, ii) =
                  TOOL::FuncCal(str_jacob_ki, gauss_sample[i],
                                gauss_sample.at(j), gauss_sample.at(k));
            }
          }

          // 获取三个权值求积
          double total_weight =
              gauss_weight.at(i) * gauss_weight.at(j) * gauss_weight.at(k);
          // 获取当前积分点的B矩阵
          Eigen::MatrixXd sub_B = Eigen::MatrixXd::Zero(6, 24);
          sub_B(0, 0) = TOOL::FuncCal(str_N1_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 3) = TOOL::FuncCal(str_N2_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 6) = TOOL::FuncCal(str_N3_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 9) = TOOL::FuncCal(str_N4_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 12) = TOOL::FuncCal(str_N5_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 15) = TOOL::FuncCal(str_N6_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 18) = TOOL::FuncCal(str_N7_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(0, 21) = TOOL::FuncCal(str_N8_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));

          sub_B(1, 1) = TOOL::FuncCal(str_N1_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 4) = TOOL::FuncCal(str_N2_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 7) = TOOL::FuncCal(str_N3_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 10) = TOOL::FuncCal(str_N4_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 13) = TOOL::FuncCal(str_N5_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 16) = TOOL::FuncCal(str_N6_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 19) = TOOL::FuncCal(str_N7_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(1, 22) = TOOL::FuncCal(str_N8_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));

          sub_B(2, 2) = TOOL::FuncCal(str_N1_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 5) = TOOL::FuncCal(str_N2_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 8) = TOOL::FuncCal(str_N3_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 11) = TOOL::FuncCal(str_N4_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 14) = TOOL::FuncCal(str_N5_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 17) = TOOL::FuncCal(str_N6_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 20) = TOOL::FuncCal(str_N7_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(2, 23) = TOOL::FuncCal(str_N8_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));

          sub_B(3, 0) = TOOL::FuncCal(str_N1_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 1) = TOOL::FuncCal(str_N1_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 3) = TOOL::FuncCal(str_N2_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 4) = TOOL::FuncCal(str_N2_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 6) = TOOL::FuncCal(str_N3_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 7) = TOOL::FuncCal(str_N3_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 9) = TOOL::FuncCal(str_N4_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 10) = TOOL::FuncCal(str_N4_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 12) = TOOL::FuncCal(str_N5_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 13) = TOOL::FuncCal(str_N5_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 15) = TOOL::FuncCal(str_N6_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 16) = TOOL::FuncCal(str_N6_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 18) = TOOL::FuncCal(str_N7_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 19) = TOOL::FuncCal(str_N7_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 21) = TOOL::FuncCal(str_N8_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(3, 22) = TOOL::FuncCal(str_N8_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));

          sub_B(4, 0) = TOOL::FuncCal(str_N1_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 2) = TOOL::FuncCal(str_N1_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 3) = TOOL::FuncCal(str_N2_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 5) = TOOL::FuncCal(str_N2_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 6) = TOOL::FuncCal(str_N3_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 8) = TOOL::FuncCal(str_N3_epsilon, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 9) = TOOL::FuncCal(str_N4_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 11) = TOOL::FuncCal(str_N4_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 12) = TOOL::FuncCal(str_N5_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 14) = TOOL::FuncCal(str_N5_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 15) = TOOL::FuncCal(str_N6_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 17) = TOOL::FuncCal(str_N6_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 18) = TOOL::FuncCal(str_N7_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 20) = TOOL::FuncCal(str_N7_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 21) = TOOL::FuncCal(str_N8_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(4, 23) = TOOL::FuncCal(str_N8_epsilon, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));

          sub_B(5, 1) = TOOL::FuncCal(str_N1_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 2) = TOOL::FuncCal(str_N1_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 4) = TOOL::FuncCal(str_N2_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 5) = TOOL::FuncCal(str_N2_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 7) = TOOL::FuncCal(str_N3_gama, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 8) = TOOL::FuncCal(str_N3_eta, gauss_sample.at(i),
                                      gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 10) = TOOL::FuncCal(str_N4_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 11) = TOOL::FuncCal(str_N4_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 13) = TOOL::FuncCal(str_N5_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 14) = TOOL::FuncCal(str_N5_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 16) = TOOL::FuncCal(str_N6_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 17) = TOOL::FuncCal(str_N6_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 19) = TOOL::FuncCal(str_N7_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 20) = TOOL::FuncCal(str_N7_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 22) = TOOL::FuncCal(str_N8_gama, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));
          sub_B(5, 23) = TOOL::FuncCal(str_N8_eta, gauss_sample.at(i),
                                       gauss_sample.at(j), gauss_sample.at(k));

          _loc_k += total_weight * sub_B.transpose() * D * sub_B *
                    jacob.determinant();
        }
      }
    }

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::MatrixXd Chexa8::GetGlobalK(const MODEL::Model &model) {
  std::map<std::string, int> dof2index;
  std::vector<std::string> vec_dof{"x",       "y",       "z",
                                   "theta_x", "theta_y", "theta_z"};

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 6; j++) {
      std::string str_dof = std::to_string(i + 1) + "_" + vec_dof.at(j);
      dof2index[str_dof] = i * 6 + j;
    }
  }
  Eigen::MatrixXd k_alldof = Eigen::MatrixXd::Zero(48, 48);
  std::map<int, std::string> k_map{
      {0, "1_x"},  {1, "1_y"},  {2, "1_z"},  {3, "2_x"},  {4, "2_y"},
      {5, "2_z"},  {6, "3_x"},  {7, "3_y"},  {8, "3_z"},  {9, "4_x"},
      {10, "4_y"}, {11, "4_z"}, {12, "5_x"}, {13, "5_y"}, {14, "5_z"},
      {15, "6_x"}, {16, "6_y"}, {17, "6_z"}, {18, "7_x"}, {19, "7_y"},
      {20, "7_z"}, {21, "8_x"}, {22, "8_y"}, {23, "8_z"}};
  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 24; j++) {
      std::string dof_row = k_map.at(i);
      std::string dof_col = k_map.at(j);
      int idx_col = dof2index.at(dof_col);
      int idx_row = dof2index.at(dof_row);
      k_alldof(idx_row, idx_col) = _loc_k(i, j);
    }
  }
  return k_alldof;
}
} // namespace COMPONENT