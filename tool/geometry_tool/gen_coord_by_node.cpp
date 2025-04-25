#include "gen_coord_by_node.h"

#include "model_tool/get_comp_by_id.h"
namespace TOOL {
boost::shared_ptr<COMPONENT::GeneralCoord>
NodesToCoord(const MODEL::Model &model, const COMPONENT::Node &n1,
             const COMPONENT::Node &n2, const COMPONENT::Node &n3) {
  try {
    // 一：通过节点获取各自对应的坐标系
    auto comp_coord1 = GetCompById(model, COMPONENT::CompBase::comp_type::coord,
                                   n1.GetCoord());
    auto base_coord1 =
        boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord1);
    if (!base_coord1) {
      throw "[ERROR]: NodesToCoord中base_coord1转换失败!";
    }
    auto comp_coord2 = GetCompById(model, COMPONENT::CompBase::comp_type::coord,
                                   n2.GetCoord());
    auto base_coord2 =
        boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord2);
    if (!base_coord2) {
      throw "[ERROR]: NodesToCoord中base_coord2转换失败!";
    }
    auto comp_coord3 = GetCompById(model, COMPONENT::CompBase::comp_type::coord,
                                   n3.GetCoord());
    auto base_coord3 =
        boost::dynamic_pointer_cast<COMPONENT::CoordBase>(comp_coord3);
    if (!base_coord3) {
      throw "[ERROR]: NodesToCoord中base_coord3转换失败!";
    }

    if (!base_coord1->GetGeneralCoord()._coord_flag &&
        !base_coord2->GetGeneralCoord()._coord_flag &&
        !base_coord3->GetGeneralCoord()._coord_flag) {
      throw "[ERROR]: NodesToCoord中有节点参考坐标系未创建";
    }

    // 二：将各个节点变换到全局坐标系下
    auto loc_n1 = n1.get_location();
    auto loc_n2 = n2.get_location();
    auto loc_n3 = n3.get_location();
    auto global_coord = boost::make_shared<COMPONENT::GlobalCoord>();
    auto trans_matrix_n1 = TransCoordToCoord(global_coord, base_coord1);
    auto trans_matrix_n2 = TransCoordToCoord(global_coord, base_coord2);
    auto trans_matrix_n3 = TransCoordToCoord(global_coord, base_coord3);

    auto glob_n1 = trans_matrix_n1 * loc_n1 + base_coord1->getORIGIN();
    auto glob_n2 = trans_matrix_n2 * loc_n2 + base_coord2->getORIGIN();
    auto glob_n3 = trans_matrix_n3 * loc_n3 + base_coord3->getORIGIN();

    // 三：通过全局坐标系下的三点坐标获得一般坐标系
    
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}
} // namespace TOOL