#pragma once
#include "component/coordinate/coordinate_base.h"
#include "component/node/node.h"
#include "fea_model/fea_model.h"
#include "trans_coord_to_coord.h"
#include "component/coordinate/global_coordinate.h"
namespace TOOL {
// 将三节点定义的坐标系转换为由原点和坐标轴定义的坐标系
boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model& model, const COMPONENT::Node& n1,
    const COMPONENT::Node& n2, const COMPONENT::Node& n3);
// 将两节点定义的坐标系转换为由原点和坐标轴定义的坐标系
boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model& model, const COMPONENT::Node& n1,
    const COMPONENT::Node& n2);
}  // namespace TOOL