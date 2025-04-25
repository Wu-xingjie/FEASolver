#include "node_to_coord.h"

#include "model_tool/get_comp_by_id.h"
namespace TOOL {
boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model& model, const COMPONENT::Node& n1,
    const COMPONENT::Node& n2, const COMPONENT::Node& n3) {
  try {
    // 通过节点获取各自对应的坐标系
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
    
    // 判断三个节点的坐标系是否都已创建
    if (base_coord1->GetGeneralCoord()._coord_flag ||base_coord2->GetGeneralCoord()._coord_flag||base_coord3->GetGeneralCoord()._coord_flag){
        throw "[ERROR]: NodesToCoord中有节点参考坐标系未创建";
    }

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}
}  // namespace TOOL