#include "CQUAD4_mindlin.h"

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

Cquad4Mindlin::Cquad4Mindlin() {
  _elem_type = ElemBase::elem_type::cquad4_ml;
  _loc_k = Eigen::MatrixXd::Zero(24, 24);
}

void Cquad4Mindlin::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
  _G3 = boost::any_cast<int>(card.at(5));
  _G4 = boost::any_cast<int>(card.at(6));
}

double Cquad4Mindlin::LenOfNode(const Eigen::Vector2d &n1,
                                const Eigen::Vector2d &n2) {
  double num0 = std::pow(n1[0] - n2[0], 2);
  double num1 = std::pow(n1[1] - n2[1], 2);
  return std::pow(num0 + num1, 0.5);
}

void Cquad4Mindlin::GenerateK(const MODEL::Model &model) {
  try {
    // 获取节点
    auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();
    auto comp_N3 = TOOL::GetCompById(model, CompBase::comp_type::node, _G3);
    auto N3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N3);
    if (!N3) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点3获取失败");
    }
    auto N3_datas = N3->get_location();
    auto comp_N4 = TOOL::GetCompById(model, CompBase::comp_type::node, _G4);
    auto N4 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N4);
    if (!N4) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点4获取失败");
    }
    auto N4_datas = N4->get_location();

    auto n1_vec = Eigen::Vector2d{N1_datas(0), N1_datas(1)};
    auto n2_vec = Eigen::Vector2d{N2_datas(0), N2_datas(1)};
    auto n3_vec = Eigen::Vector2d{N3_datas(0), N3_datas(1)};
    auto n4_vec = Eigen::Vector2d{N4_datas(0), N4_datas(1)};

    std::array<double, 4> x_arr{N1_datas(0), N2_datas(0), N3_datas(0),
                                N4_datas(0)};
    std::array<double, 4> y_arr{N1_datas(1), N2_datas(1), N3_datas(1),
                                N4_datas(1)};
    std::array<std::array<double, 4>, 2> point_arr{x_arr, y_arr};

    // 获取单元属性
    auto comp_prop = TOOL::GetCompById(model, CompBase::comp_type::prop, _pid);
    auto base_prop = boost::dynamic_pointer_cast<PropertyBase>(comp_prop);
    if (!base_prop) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>属性基类获取失败");
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
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>材料基类获取失败");
    }
    auto mat_data1 = base_mat1->GetMatDate();
    auto mat_info1 = TOOL::DealENuG(mat_data1);
    if (mat_info1.empty()) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>材料获取失败");
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
            "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>mat2材料基类获取失败");
      }
      auto mat_data2 = base_mat2->GetMatDate();
      auto mat_info2 = TOOL::DealENuG(mat_data2);
      if (mat_info2.empty()) {
        throw std::runtime_error(
            "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>mat2材料获取失败");
      }
      E2 = mat_info2.at(0);
      NU2 = mat_info2.at(1);
      G2 = mat_info2.at(2);
    }

    // 物理矩阵
    // 膜行为物理矩阵
    Eigen::MatrixXd D_plane = Eigen::MatrixXd::Zero(3, 3);
    D_plane(0, 0) = 1;
    D_plane(1, 1) = 1;
    D_plane(1, 0) = NU1;
    D_plane(0, 1) = NU1;
    D_plane(2, 2) = (1 - NU1) / 2.0;
    D_plane *= E1 / (1 - std::pow(NU1, 2));
    // 板弯曲的物理矩阵
    // 内力矩物理矩阵
    Eigen::MatrixXd D_bend = Eigen::MatrixXd::Zero(5, 5);
    Eigen::MatrixXd D_bend_b = Eigen::MatrixXd::Zero(3, 3);
    D_bend_b(0, 0) = 1;
    D_bend_b(0, 1) = NU2;
    D_bend_b(1, 0) = NU2;
    // D_bend_b(1, 1) = 1;
    D_bend_b(2, 2) = (1 - NU2) * 0.5;
    D_bend_b *= E2 * t * t * t / (12 * (1 - NU2 * NU2));
    // 剪力物理矩阵
    Eigen::MatrixXd D_bend_s = Eigen::MatrixXd::Zero(2, 2);
    D_bend_s(0, 0) = 1;
    D_bend_s(1, 1) = 1;
    D_bend_s *= 5 * E2 * t / (12 * (1 + NU2));
    D_bend.block<3, 3>(0, 0) = D_bend_b;
    D_bend.block<2, 2>(3, 3) = D_bend_s;

    TOOL::DisplayMatrixXd(D_bend, "D_bend");

    // 获取高斯积分点和积分权值
    // TODO:单元如果需要采取减缩积分，可以修改变量gauss_num
    int gauss_num = 3;
    auto gauss_sample = TOOL::GetGaussSampPoint(gauss_num);
    auto gauss_weight = TOOL::GetGaussWeightVal(gauss_num);

    // 平面应力计算时以字符串的形式表示出形函数对自然坐标的偏导
    std::string str_N1_epsilon = "-0.25*(1-y)";
    std::string str_N1_eta = "-0.25*(1-x)";
    std::string str_N2_epsilon = "0.25*(1-y)";
    std::string str_N2_eta = "-0.25*(1+x)";
    std::string str_N3_epsilon = "0.25*(1+y)";
    std::string str_N3_eta = "0.25*(1+x)";
    std::string str_N4_epsilon = "-0.25*(1+y)";
    std::string str_N4_eta = "0.25*(1-x)";

    std::array<std::string, 4> Ni_epsilon{str_N1_epsilon, str_N2_epsilon,
                                          str_N3_epsilon, str_N4_epsilon};
    std::array<std::string, 4> Ni_eta{str_N1_eta, str_N2_eta, str_N3_eta,
                                      str_N4_eta};
    std::array<std::array<std::string, 4>, 2> arr_Ni_partial{Ni_epsilon,
                                                             Ni_eta};

    Eigen::MatrixXd k_plane = Eigen::MatrixXd::Zero(8, 8);
    Eigen::MatrixXd k_bend = Eigen::MatrixXd::Zero(12, 12);
    for (int i = 0; i < gauss_num; i++) {
      for (int j = 0; j < gauss_num; j++) {
        //  计算jacob矩阵行列式
        Eigen::Matrix2d jacob = Eigen::Matrix2d::Zero();
        for (int kk = 0; kk < 2; kk++) {
          for (int ii = 0; ii < 2; ii++) {
            std::string str_jacob_ki = "";
            for (int jj = 0; jj < 4; jj++) {
              str_jacob_ki += "+(" + arr_Ni_partial.at(kk).at(jj) + ")*" +
                              std::to_string(point_arr.at(ii).at(jj));
            }
            jacob(kk, ii) = TOOL::FuncCal(str_jacob_ki, gauss_sample[i],
                                          gauss_sample.at(j));
          }
        }
        auto jacob_inv = jacob.inverse();

        // =============== 平面应力行为 ===============

        // 计算形函数Ni对x,y,z的偏导数
        double N1_epsilon = TOOL::FuncCal(str_N1_epsilon, gauss_sample.at(i),
                                          gauss_sample.at(j));
        double N1_eta =
            TOOL::FuncCal(str_N1_eta, gauss_sample.at(i), gauss_sample.at(j));
        Eigen::Vector2d N1_X_nature(N1_epsilon, N1_eta);
        Eigen::Vector2d N1_X = jacob_inv * N1_X_nature;

        double N2_epsilon = TOOL::FuncCal(str_N2_epsilon, gauss_sample.at(i),
                                          gauss_sample.at(j));
        double N2_eta =
            TOOL::FuncCal(str_N2_eta, gauss_sample.at(i), gauss_sample.at(j));
        Eigen::Vector2d N2_X_nature(N2_epsilon, N2_eta);
        Eigen::Vector2d N2_X = jacob_inv * N2_X_nature;

        double N3_epsilon = TOOL::FuncCal(str_N3_epsilon, gauss_sample.at(i),
                                          gauss_sample.at(j));
        double N3_eta =
            TOOL::FuncCal(str_N3_eta, gauss_sample.at(i), gauss_sample.at(j));
        Eigen::Vector2d N3_X_nature(N3_epsilon, N3_eta);
        Eigen::Vector2d N3_X = jacob_inv * N3_X_nature;

        double N4_epsilon = TOOL::FuncCal(str_N4_epsilon, gauss_sample.at(i),
                                          gauss_sample.at(j));
        double N4_eta =
            TOOL::FuncCal(str_N4_eta, gauss_sample.at(i), gauss_sample.at(j));
        Eigen::Vector2d N4_X_nature(N4_epsilon, N4_eta);
        Eigen::Vector2d N4_X = jacob_inv * N4_X_nature;

        // 获取三个权值求积
        double total_weight = gauss_weight.at(i) * gauss_weight.at(j);

        // 获取当前积分点的B矩阵
        Eigen::MatrixXd sub_B = Eigen::MatrixXd::Zero(3, 8);
        sub_B(0, 0) = N1_X(0);
        sub_B(0, 2) = N2_X(0);
        sub_B(0, 4) = N3_X(0);
        sub_B(0, 6) = N4_X(0);

        sub_B(0, 1) = N1_X(1);
        sub_B(0, 3) = N2_X(1);
        sub_B(0, 5) = N3_X(1);
        sub_B(0, 7) = N4_X(1);

        sub_B(0, 0) = N1_X(1);
        sub_B(0, 1) = N1_X(0);
        sub_B(0, 2) = N2_X(1);
        sub_B(0, 3) = N2_X(0);
        sub_B(0, 4) = N3_X(1);
        sub_B(0, 5) = N3_X(0);
        sub_B(0, 6) = N4_X(1);
        sub_B(0, 7) = N4_X(0);

        k_plane += t * total_weight * sub_B.transpose() * D_plane * sub_B *
                   jacob.determinant();
      }
    }
    // =============== 基于mindlin理论的板弯曲行为 ===============
    int gauss_num_bend = 3;
    auto gauss_sample_bend = TOOL::GetGaussSampPoint(gauss_num_bend);
    auto gauss_weight_bend = TOOL::GetGaussWeightVal(gauss_num_bend);
    for (int i = 0; i < gauss_num_bend; i++) {
      for (int j = 0; j < gauss_num_bend; j++) {
        //  计算jacob矩阵行列式
        Eigen::Matrix2d jacob = Eigen::Matrix2d::Zero();
        for (int kk = 0; kk < 2; kk++) {
          for (int ii = 0; ii < 2; ii++) {
            std::string str_jacob_ki = "";
            for (int jj = 0; jj < 4; jj++) {
              str_jacob_ki += "+(" + arr_Ni_partial.at(kk).at(jj) + ")*" +
                              std::to_string(point_arr.at(ii).at(jj));
            }
            jacob(kk, ii) = TOOL::FuncCal(str_jacob_ki, gauss_sample_bend[i],
                                          gauss_sample_bend.at(j));
          }
        }
        auto jacob_inv = jacob.inverse();

        // 计算形函数Ni对x,y,z的偏导数
        double N1_epsilon = TOOL::FuncCal(
            str_N1_epsilon, gauss_sample_bend.at(i), gauss_sample_bend.at(j));
        double N1_eta = TOOL::FuncCal(str_N1_eta, gauss_sample_bend.at(i),
                                      gauss_sample_bend.at(j));
        Eigen::Vector2d N1_X_nature(N1_epsilon, N1_eta);
        Eigen::Vector2d N1_X = jacob_inv * N1_X_nature;

        double N2_epsilon = TOOL::FuncCal(
            str_N2_epsilon, gauss_sample_bend.at(i), gauss_sample_bend.at(j));
        double N2_eta = TOOL::FuncCal(str_N2_eta, gauss_sample_bend.at(i),
                                      gauss_sample_bend.at(j));
        Eigen::Vector2d N2_X_nature(N2_epsilon, N2_eta);
        Eigen::Vector2d N2_X = jacob_inv * N2_X_nature;

        double N3_epsilon = TOOL::FuncCal(
            str_N3_epsilon, gauss_sample_bend.at(i), gauss_sample_bend.at(j));
        double N3_eta = TOOL::FuncCal(str_N3_eta, gauss_sample_bend.at(i),
                                      gauss_sample_bend.at(j));
        Eigen::Vector2d N3_X_nature(N3_epsilon, N3_eta);
        Eigen::Vector2d N3_X = jacob_inv * N3_X_nature;

        double N4_epsilon = TOOL::FuncCal(
            str_N4_epsilon, gauss_sample_bend.at(i), gauss_sample_bend.at(j));
        double N4_eta = TOOL::FuncCal(str_N4_eta, gauss_sample_bend.at(i),
                                      gauss_sample_bend.at(j));
        Eigen::Vector2d N4_X_nature(N4_epsilon, N4_eta);
        Eigen::Vector2d N4_X = jacob_inv * N4_X_nature;

        // 获取三个权值求积
        double total_weight = gauss_weight_bend.at(i) * gauss_weight_bend.at(j);

        // =============== 基于mindlin理论的板弯曲行为 ===============
        std::string str_N1 = "0.25*(1-x)*(1-y)";
        std::string str_N2 = "0.25*(1+x)*(1-y)";
        std::string str_N3 = "0.25*(1+x)*(1+y)";
        std::string str_N4 = "0.25*(1-x)*(1+y)";

        double N1 = TOOL::FuncCal(str_N1, gauss_sample_bend.at(i),
                                  gauss_sample_bend.at(j));
        double N2 = TOOL::FuncCal(str_N2, gauss_sample_bend.at(i),
                                  gauss_sample_bend.at(j));
        double N3 = TOOL::FuncCal(str_N3, gauss_sample_bend.at(i),
                                  gauss_sample_bend.at(j));
        double N4 = TOOL::FuncCal(str_N4, gauss_sample_bend.at(i),
                                  gauss_sample_bend.at(j));
        std::array<double, 4> Ns{N1, N2, N3, N4};

        Eigen::MatrixXd sub_bend_B = Eigen::MatrixXd::Zero(5, 12);
        std::array<Eigen::Vector2d, 4> Ni_Xs{N1_X, N2_X, N3_X, N4_X};
        for (int bi = 0; bi < 4; bi++) {
          auto Ni_X = Ni_Xs.at(bi);
          auto Ni = Ns.at(bi);
          Eigen::MatrixXd temp_B = Eigen::MatrixXd::Zero(5, 3);
          temp_B(0, 2) = Ni_X(0);
          temp_B(1, 1) = -Ni_X(1);
          temp_B(2, 1) = -Ni_X(0);
          temp_B(2, 2) = Ni_X(1);
          temp_B(3, 0) = Ni_X(0);
          temp_B(4, 0) = Ni_X(1);
          temp_B(3, 2) = Ni;
          temp_B(4, 1) = -Ni;
          sub_bend_B.block<5, 3>(0, 3 * bi) = temp_B;
          TOOL::DisplayMatrixXd(temp_B, "temp_B");
          TOOL::DisplayMatrixXd(sub_bend_B, "sub_bend_B");
        }
        k_bend += total_weight * jacob.determinant() * sub_bend_B.transpose() *
                  D_bend * sub_bend_B;
      }
    }

    // 设置总刚度矩阵
    // 1: 把板弯刚度矩阵和膜刚度矩阵扩容成24*24的全自由度刚度矩阵
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
        std::string dof_row = plane_k_map.at(i);
        std::string dof_col = plane_k_map.at(j);
        int idx_col = dof2index.at(dof_col);
        int idx_row = dof2index.at(dof_row);
        plane_k_alldof(idx_row, idx_col) = k_plane(i, j);
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
        std::string dof_row = bend_k_map.at(i);
        std::string dof_col = bend_k_map.at(j);
        int idx_col = dof2index.at(dof_col);
        int idx_row = dof2index.at(dof_row);
        bend_k_alldof(idx_row, idx_col) = k_bend(i, j);
      }
    }
    // TOOL::DisplayMatrixXd(k_plane, "k_plane", true);
    // TOOL::DisplayMatrixXd(plane_k_alldof, "plane_k_alldof", true);
    // TOOL::DisplayMatrixXd(k_bend, "k_bend", true);
    // TOOL::DisplayMatrixXd(bend_k_alldof, "bend_k_alldof", true);

    // 2:将扩容后的板弯刚度矩阵和膜刚度矩阵相加；
    _loc_k = plane_k_alldof + bend_k_alldof;
    // TOOL::DisplayMatrixXd(_loc_k, "_loc_k", true);
  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::MatrixXd Cquad4Mindlin::GetGlobalK(const MODEL::Model &model) {
  return _loc_k;
}
} // namespace COMPONENT