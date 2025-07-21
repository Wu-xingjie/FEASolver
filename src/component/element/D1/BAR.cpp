#include "BAR.h"

#include <cmath>
#include <exception>

#include "component/coordinate/global_coordinate.h"
#include "geometry_tool/gen_coord_by_node.h"
#include "geometry_tool/length_node_to_node.h"
#include "model_tool/deal_E_NU_G.h"
#include "model_tool/get_comp_by_id.h"

namespace COMPONENT {

BAR::BAR() {
  _elem_type = ElemBase::elem_type::bar;
  _loc_k = Eigen::MatrixXd::Zero(12, 12);
}

void BAR::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
  _origin_vec = Eigen::Vector3d::Zero(3);
  _origin_vec[0] = boost::any_cast<double>(card.at(5));
  _origin_vec[1] = boost::any_cast<double>(card.at(6));
  _origin_vec[2] = boost::any_cast<double>(card.at(7));
}

void BAR::GenerateK(const MODEL::Model &model) {
  // 获取节点信息
  try {
    // 获取节点
    auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error("[ERROR]:func(BAR::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error("[ERROR]:func(BAR::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();
    // TODO: 由于坐标系暂未处理，此处假定节点都定义在全局坐标系下
    //  获取单元局部坐标系
    Eigen::Vector3f loc_cord = Eigen::Vector3f::Zero();
    loc_cord.x() = N2_datas.x() - N1_datas.x();
    loc_cord.y() = N2_datas.y() - N1_datas.y();
    loc_cord.z() = N2_datas.z() - N1_datas.z();
    // 杆单元长度
    double l = TOOL::LenNode2Node(model, N1, N2);

    // 获取单元属性
    auto comp_prop = TOOL::GetCompById(model, CompBase::comp_type::prop, _pid);
    auto base_prop = boost::dynamic_pointer_cast<PropertyBase>(comp_prop);
    if (!base_prop) {
      throw std::runtime_error(
          "[ERROR]:func(BAR::GenerateK)>>>属性基类获取失败");
    }
    auto prop_datas = base_prop->GetPropDate();
    int mid = boost::any_cast<int>(prop_datas.at("mid"));
    double a = boost::any_cast<double>(prop_datas.at("a"));
    double I1 = boost::any_cast<double>(prop_datas.at("I1"));
    double I2 = boost::any_cast<double>(prop_datas.at("I2"));
    double J = boost::any_cast<double>(prop_datas.at("J"));

    // 获取单元材料
    auto comp_mat = TOOL::GetCompById(model, CompBase::comp_type::mat, mid);
    auto base_mat = boost::dynamic_pointer_cast<MaterialBase>(comp_mat);
    if (!base_mat) {
      throw std::runtime_error(
          "[ERROR]:func(BAR::GenerateK)>>>材料基类获取失败");
    }
    auto mat_data = base_mat->GetMatDate();
    auto mat_info = TOOL::DealENuG(mat_data);
    if (mat_info.empty()) {
      throw std::runtime_error("[ERROR]:func(BAR::GenerateK)>>>材料获取失败");
    }
    double E = mat_info.at(0);
    double G = mat_info.at(2);

    // 生成局部坐标系下的单元刚度矩阵
    _loc_k(0, 0) = E * a / l;
    _loc_k(0, 6) = -E * a / l;
    _loc_k(6, 0) = -E * a / l;

    _loc_k(1, 1) = 12 * E * I2 / std::pow(l, 3);
    _loc_k(5, 1) = 6 * E * I2 / std::pow(l, 2);
    _loc_k(1, 5) = 6 * E * I2 / std::pow(l, 2);
    _loc_k(7, 1) = -12 * E * I2 / std::pow(l, 3);
    _loc_k(1, 7) = -12 * E * I2 / std::pow(l, 3);
    _loc_k(11, 1) = 6 * E * I2 / std::pow(l, 2);
    _loc_k(1, 11) = 6 * E * I2 / std::pow(l, 2);

    _loc_k(2, 2) = 12 * E * I1 / std::pow(l, 3);
    _loc_k(4, 2) = -6 * E * I1 / std::pow(l, 2);
    _loc_k(2, 4) = -6 * E * I1 / std::pow(l, 2);
    _loc_k(8, 2) = 12 * E * I1 / std::pow(l, 3);
    _loc_k(2, 8) = 12 * E * I1 / std::pow(l, 3);
    _loc_k(10, 2) = -6 * E * I1 / std::pow(l, 2);
    _loc_k(2, 10) = -6 * E * I1 / std::pow(l, 2);

    _loc_k(3, 3) = G * J / l;
    _loc_k(9, 3) = -G * J / l;
    _loc_k(3, 9) = -G * J / l;

    _loc_k(4, 4) = 4 * E * I1 / l;
    _loc_k(8, 4) = 6 * E * I1 / std::pow(l, 2);
    _loc_k(4, 8) = 6 * E * I1 / std::pow(l, 2);
    _loc_k(10, 4) = 2 * E * I1 / l;
    _loc_k(4, 10) = 2 * E * I1 / l;

    _loc_k(5, 5) = 4 * E * I2 / l;
    _loc_k(7, 5) = -6 * E * I2 / std::pow(l, 2);
    _loc_k(5, 7) = -6 * E * I2 / std::pow(l, 2);
    _loc_k(11, 5) = 2 * E * I2 / l;
    _loc_k(5, 11) = 2 * E * I2 / l;

    _loc_k(6, 6) = E * a / l;

    _loc_k(7, 7) = 12 * E * I2 / std::pow(l, 3);
    _loc_k(11, 7) = -6 * E * I2 / std::pow(l, 2);
    _loc_k(7, 11) = -6 * E * I2 / std::pow(l, 2);

    _loc_k(8, 8) = 12 * E * I1 / std::pow(l, 3);
    _loc_k(10, 8) = 6 * E * I1 / std::pow(l, 2);
    _loc_k(8, 10) = 6 * E * I1 / std::pow(l, 2);

    _loc_k(9, 9) = G * J / l;

    _loc_k(10, 10) = 4 * E * I1 / l;

    _loc_k(11, 11) = 4 * E * I2 / l;

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::MatrixXd BAR::GetGlobalK(const MODEL::Model &model) {
  // 获取全局坐标系
  auto global_coord =
      boost::make_shared<COMPONENT::GlobalCoord>()->GetGeneralCoord();
  // 获取单元局部坐标系
  // x轴
  auto coord_x = TOOL::NodesToCoord(model, _G1, _G2)->_vec1;
  // z轴
  auto coord_z = coord_x.cross(_origin_vec);
  // y轴
  auto coord_y = coord_z.cross(coord_x);

  // 获取局部坐标系到全局坐标系的坐标变换矩阵
  // 坐标原点
  auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
  auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
  if (!N1) {
    throw std::runtime_error("[ERROR]:func(BAR::GenerateK)>>>节点1获取失败");
  }
  auto N1_datas = N1->get_location();
  // 创建局部坐标系的一般坐标系
  GeneralCoord loc_coord;
  loc_coord._coord_origin = N1_datas;
  loc_coord._vec1 = coord_x.normalized();
  loc_coord._vec2 = coord_y.normalized();
  loc_coord._vec3 = coord_z.normalized();
  std::cout << "_vec1: " << std::endl << loc_coord._vec1 << std::endl;
  std::cout << "_vec2: " << std::endl << loc_coord._vec2 << std::endl;
  std::cout << "_vec3: " << std::endl << loc_coord._vec3 << std::endl;
  loc_coord._dim_type = GeneralCoord::gen_coord_type::dim3;
  // 获取坐标变换矩阵
  auto trans_matrix_block = TOOL::TransCoordToCoord(global_coord, loc_coord);
  std::cout << "trans_matrix_block:" << std::endl
            << trans_matrix_block << std::endl;
  Eigen::MatrixXd trans_matrix = Eigen::MatrixXd::Zero(12, 12);
  trans_matrix.block<3, 3>(0, 0) = trans_matrix_block;
  trans_matrix.block<3, 3>(3, 3) = trans_matrix_block;
  trans_matrix.block<3, 3>(6, 6) = trans_matrix_block;
  trans_matrix.block<3, 3>(9, 9) = trans_matrix_block;

  // 全局坐标系下单元刚度矩阵
  Eigen::MatrixXd global_k;
  std::cout << "trans_matrix:" << std::endl << trans_matrix << std::endl;
  global_k = trans_matrix * _loc_k * trans_matrix.transpose();
  std::cout << "global_k:" << std::endl << global_k << std::endl;
  return global_k;
}

} // namespace COMPONENT