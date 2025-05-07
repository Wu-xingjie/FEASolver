#include "ROD.h"

#include "component/material/material_base.h"
#include "component/node/node.h"
#include "component/property/property_base.h"
#include "geometry_tool/gen_coord_by_node.h"
#include "geometry_tool/length_node_to_node.h"
#include "model_tool/deal_E_NU_G.h"
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
      throw "节点1获取失败";
    }
    auto N1_datas = N1->get_location();
    auto comp_N2 = TOOL::GetCompById(model, CompBase::comp_type::node, _G2);
    auto N2 = boost::dynamic_pointer_cast<COMPONENT::Node>(comp_N2);
    if (!N2) {
      throw "节点2获取失败";
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
      throw "属性基类获取失败";
    }
    double A = base_prop->GetCrossArea();
    int MID = base_prop->GetMatId();
    // 获取单元材料
    auto comp_mat = TOOL::GetCompById(model, CompBase::comp_type::mat, MID);
    auto base_mat = boost::dynamic_pointer_cast<MaterialBase>(comp_mat);
    if (!base_mat) {
      throw "材料基类获取失败";
    }
    auto E = base_mat->GetE();
    auto NU = base_mat->GetNU();
    auto G = base_mat->GetG();
    auto mat_info = TOOL::DealENuG(E, NU, G);
    if (mat_info.empty()) {
      throw "材料获取失败";
    }
    // 生成单元刚度矩阵
    double num = (mat_info.at(0) * A) / l;
    _loc_k << 1, -1, -1, 1;
    _loc_k = _loc_k * num;

  } catch (const char *e) {
    std::cout << "[ERROR]:单元" << _id << ": " << e << '\n';
  }
}

Eigen::Matrix2d ROD::GetK() { return _loc_k; }

Eigen::MatrixXd ROD::GetGlobalK(const MODEL::Model &model) {
  Eigen::MatrixXd global_k = Eigen::MatrixXd::Zero(2, 2);
  // 获取局部坐标系和全局坐标系
  auto loc_coord = *TOOL::NodesToCoord(model, _G1, _G2);
  auto global_coord =
      boost::make_shared<COMPONENT::GlobalCoord>()->GetGeneralCoord();
  // 获取局部坐标系到全局坐标系的坐标变换矩阵
  auto trans_matrix = TOOL::TransCoordToCoord(global_coord, loc_coord);
  return global_k;
}

}  // namespace COMPONENT