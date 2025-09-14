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

    auto n1_vec = Eigen::Vector2d{N1_datas(0), N1_datas(1)};
    auto n2_vec = Eigen::Vector2d{N2_datas(0), N2_datas(1)};
    auto n3_vec = Eigen::Vector2d{N3_datas(0), N3_datas(1)};
    auto n4_vec = Eigen::Vector2d{N4_datas(0), N4_datas(1)};
    auto n5_vec = Eigen::Vector2d{N5_datas(0), N5_datas(1)};
    auto n6_vec = Eigen::Vector2d{N6_datas(0), N6_datas(1)};
    auto n7_vec = Eigen::Vector2d{N7_datas(0), N7_datas(1)};
    auto n8_vec = Eigen::Vector2d{N8_datas(0), N8_datas(1)};

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

    // 拿到高斯积分的采样点和积分权值
    auto gauss_sample = TOOL::GetGaussSampPoint(3);
    auto gauss_weight = TOOL::GetGaussWeightVal(3);

    // 获取单元刚度矩阵

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
          // 获取三个权值求积
          double total_weight =
              gauss_weight.at(i) * gauss_weight.at(j) * gauss_weight.at(k);
          // 获取当前积分点的B矩阵
          Eigen::MatrixXd sub_B = Eigen::MatrixXd::Zero(24, 24);
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

          _loc_k += total_weight * sub_B.transpose() * D * sub_B;
        }
      }
    }

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

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
  global_k = trans_matrix.transpose() * _loc_k * trans_matrix;

  // TOOL::DisplayMatrixXd(global_k, "global_k");
  return global_k;
}
} // namespace COMPONENT