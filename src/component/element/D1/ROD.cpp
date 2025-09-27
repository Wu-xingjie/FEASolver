#include "ROD.h"

#include "component/material/material_base.h"
#include "component/node/node.h"
#include "component/property/property_base.h"
#include "geometry_tool/cos_of_vectors.h"
#include "geometry_tool/gen_coord_by_node.h"
#include "geometry_tool/length_node_to_node.h"
#include "model_tool/deal_E_NU_G.h"
#include "model_tool/display_matrixXd.h"
#include "model_tool/get_comp_by_id.h"

namespace COMPONENT {

void ROD::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _pid = boost::any_cast<int>(card.at(2));
  _G1 = boost::any_cast<int>(card.at(3));
  _G2 = boost::any_cast<int>(card.at(4));
}

void ROD::GenerateK(const MODEL::Model &model) {
  try {
    // 获取节点
    auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
    auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
    if (!N1) {
      throw std::runtime_error("[ERROR]:func(ROD::GenerateK)>>>节点1获取失败");
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw std::runtime_error("[ERROR]:func(ROD::GenerateK)>>>节点2获取失败");
    }
    auto N2_datas = N2->get_location();
    // TODO: 由于坐标系暂未处理，此处假定节点都定义在全局坐标系下
    //  获取杆单元局部坐标系
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
          "[ERROR]:func(ROD::GenerateK)>>>属性基类获取失败");
    }
    auto prop_datas = base_prop->GetPropDate();
    int MID = boost::any_cast<int>(prop_datas.at("mid"));
    double A = boost::any_cast<double>(prop_datas.at("a"));

    // 获取单元材料
    auto comp_mat = TOOL::GetCompById(model, CompBase::comp_type::mat, MID);
    auto base_mat = boost::dynamic_pointer_cast<MaterialBase>(comp_mat);
    if (!base_mat) {
      throw std::runtime_error(
          "[ERROR]:func(ROD::GenerateK)>>>材料基类获取失败");
    }

    auto mat_data = base_mat->GetMatDate();
    auto mat_info = TOOL::DealENuG(mat_data);
    if (mat_info.empty()) {
      throw std::runtime_error("[ERROR]:func(ROD::GenerateK)>>>材料获取失败");
    }
    // 生成单元刚度矩阵
    Eigen::MatrixXd temp_k = Eigen::MatrixXd::Zero(2, 2);
    double num = (mat_info.at(0) * A) / l;
    temp_k << 1, -1, -1, 1;
    temp_k = temp_k * num;

    // 1.2: 将单元刚度矩阵扩容
    std::map<std::string, int> dof2index;
    std::vector<std::string> vec_dof{"x",       "y",       "z",
                                     "theta_x", "theta_y", "theta_z"};
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 6; j++) {
        std::string str_dof = std::to_string(i + 1) + "_" + vec_dof.at(j);
        dof2index[str_dof] = i * 6 + j;
      }
    }

    std::map<int, std::string> k_map{{0, "1_x"}, {1, "2_x"}};
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        std::string dof_row = k_map.at(i);
        std::string dof_col = k_map.at(j);
        int idx_col = dof2index.at(dof_col);
        int idx_row = dof2index.at(dof_row);
        _loc_k(idx_row, idx_col) = temp_k(i, j);
      }
    }
    TOOL::DisplayMatrixXd(_loc_k, "loc_k");

  } catch (const char *e) {
    std::cout << "[ERROR]:func(ROD::GenerateK)>>>单元" << _id << ": " << e
              << '\n';
  }
}

Eigen::MatrixXd ROD::GetK() { return _loc_k; }

Eigen::MatrixXd ROD::GetGlobalK(const MODEL::Model &model) {
  Eigen::MatrixXd global_k = Eigen::MatrixXd::Zero(12, 12);
  // 获取全局坐标系和单元局部坐标系
  auto global_coord =
      boost::make_shared<COMPONENT::GlobalCoord>()->GetGeneralCoord();

  // 获取节点
  auto comp_N1 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
  auto N1 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N1);
  if (!N1) {
    throw std::runtime_error("[ERROR]:func(ROD::GetGlobalK)>>>节点1获取失败");
  }
  auto N1_datas = N1->get_location();
  auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G1);
  auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
  if (!N2) {
    throw std::runtime_error("[ERROR]:func(ROD::GetGlobalK)>>>节点1获取失败");
  }
  auto N2_datas = N2->get_location();

  // 创建_G1到_G2的向量
  Eigen::Vector3d vec_1_2{N2_datas(0) - N1_datas(0), N2_datas(1) - N1_datas(1),
                          N2_datas(2) - N1_datas(2)};

  // 创建一个临时的三维笛卡尔坐标系作为局部坐标系
  Eigen::Vector3d temp_point{N2_datas(0), N2_datas(1), N2_datas(2) + 1};

  // 获取单元局部坐标系
  // x轴
  auto coord_x = TOOL::NodesToCoord(model, _G1, _G2)->_vec1;
  // z轴
  auto coord_z = coord_x.cross(temp_point);
  // y轴
  auto coord_y = coord_z.cross(coord_x);

  // 获取局部坐标系到全局坐标系的坐标变换矩阵
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
  std::cout << "trans_matrix:" << std::endl << trans_matrix << std::endl;
  global_k = trans_matrix * _loc_k * trans_matrix.transpose();
  std::cout << "global_k:" << std::endl << global_k << std::endl;
  return global_k;

  // 获取单元坐标系向量和全局坐标系之间的方向余弦
  // auto cos_l_x = TOOL::CosOfVecs(global_coord._vec1, vec_loc);
  // auto cos_l_y = TOOL::CosOfVecs(global_coord._vec2, vec_loc);
  // auto cos_l_z = TOOL::CosOfVecs(global_coord._vec3, vec_loc);
  // // 创建变换矩阵
  // Eigen::MatrixXd trans_matrix = Eigen::MatrixXd::Zero(6, 2);
  // trans_matrix(0, 0) = cos_l_x;
  // trans_matrix(1, 0) = cos_l_y;
  // trans_matrix(2, 0) = cos_l_z;
  // trans_matrix(3, 0) = 0;
  // trans_matrix(4, 0) = 0;
  // trans_matrix(5, 0) = 0;
  // trans_matrix(0, 1) = 0;
  // trans_matrix(1, 1) = 0;
  // trans_matrix(2, 1) = 0;
  // trans_matrix(3, 1) = cos_l_x;
  // trans_matrix(4, 1) = cos_l_y;
  // trans_matrix(5, 1) = cos_l_z;

  // // 获取全局坐标系下的单元刚度矩阵
  // global_k = trans_matrix * _loc_k * trans_matrix.transpose();
  // return global_k;
}

} // namespace COMPONENT