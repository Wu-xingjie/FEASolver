#include "uniformly_distribute_surf_load.h"

#include "math_tool/func_expr/func_cal.h"
#include "math_tool/gauss_integral.h"
#include "model_tool/get_comp_by_id.h"

namespace COMPONENT {
void UniDistributeSurfLoad::SetComp(const file_data &datas) {
  if (datas.size() > 1) {
    throw std::runtime_error(
        "[ERROR]:func(UniDistributeSurfLoad::SetComp)>>>"
        "均布面载荷暂不支持作用于节点数超过6的单元面");
  }
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _load_x = boost::any_cast<int>(card.at(2));
  _load_y = boost::any_cast<int>(card.at(3));
  _load_z = boost::any_cast<int>(card.at(4));
  for (int i = 5; i < 11; i++) {
    int nid = boost::any_cast<int>(card.at(i));
    _surf_nids.push_back(nid);
  }
}

void UniDistributeSurfLoad::GenLoadVec(const MODEL::Model &model) {
  // FIXME:暂时只考虑施加在四边形面上的均布载荷
  if (_surf_nids.size() == 4) {
    // 获取载荷作用节点
    auto comp_N1 =
        TOOL::GetCompById(model, CompBase::comp_type::node, _surf_nids.at(0));
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 =
        TOOL::GetCompById(model, CompBase::comp_type::node, _surf_nids.at(1));
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();
    auto comp_N3 =
        TOOL::GetCompById(model, CompBase::comp_type::node, _surf_nids.at(2));
    auto N3 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N3);
    if (!N3) {
      throw std::runtime_error(
          "[ERROR]:func(Cquad4Mindlin::GenerateK)>>>节点3获取失败");
    }
    auto N3_datas = N3->get_location();
    auto comp_N4 =
        TOOL::GetCompById(model, CompBase::comp_type::node, _surf_nids.at(3));
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

    Eigen::Vector3d load_vec{_load_x, _load_y, _load_z};  // 载荷列阵

    // 算式预处理
    std::string str_N1 = "0.25*(1-x)*(1-y)";
    std::string str_N2 = "0.25*(1+x)*(1-y)";
    std::string str_N3 = "0.25*(1+x)*(1+y)";
    std::string str_N4 = "0.25*(1-x)*(1+y)";

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
    // 获取高斯积分所需的采样点和积分权值
    // TODO:单元如果需要采取减缩积分，可以修改变量gauss_num
    int gauss_num = 3;
    auto gauss_sample = TOOL::GetGaussSampPoint(gauss_num);
    auto gauss_weight = TOOL::GetGaussWeightVal(gauss_num);

    for (int i = 0; i < gauss_num; i++) {
      for (int j = 0; j < gauss_num; j++) {
        // 计算jacob矩阵
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

        Eigen::MatrixXd N_T = Eigen::MatrixXd::Zero(3, 12);
        double N1 =
            TOOL::FuncCal(str_N1, gauss_sample.at(i), gauss_sample.at(j));
        N_T(0, 0) = N1;
        N_T(1, 1) = N1;
        N_T(2, 2) = N1;
        double N2 =
            TOOL::FuncCal(str_N2, gauss_sample.at(i), gauss_sample.at(j));
        N_T(0, 3) = N2;
        N_T(1, 4) = N2;
        N_T(2, 5) = N2;
        double N3 =
            TOOL::FuncCal(str_N3, gauss_sample.at(i), gauss_sample.at(j));
        N_T(0, 6) = N3;
        N_T(1, 7) = N3;
        N_T(2, 8) = N3;
        double N4 =
            TOOL::FuncCal(str_N4, gauss_sample.at(i), gauss_sample.at(j));
        N_T(0, 9) = N4;
        N_T(1, 10) = N4;
        N_T(2, 11) = N4;

        _load_vec += gauss_weight.at(i) * gauss_weight.at(j) * N_T * load_vec *
                     jacob.determinant();
      }
    }

  } else {
    throw std::runtime_error(
        "[ERROR]:func(UniDistributeSurfLoad::GenLoadVec)>>"
        ">当前仅考虑施加在四边形面上的均布载荷");
  }
}

}  // namespace COMPONENT