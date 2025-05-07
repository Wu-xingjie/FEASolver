#pragma once
#include "component/coordinate/coordinate_base.h"
#include "component/coordinate/global_coordinate.h"
#include "component/node/node.h"
#include "fea_model/fea_model.h"
#include "trans_coord_to_coord.h"
namespace TOOL {
// 将三节点定义的坐标系转换为由原点和坐标轴定义的坐标系
boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model &model, const int &nid1, const int &nid2,
    const int &nid3);
// 将两节点定义的坐标系转换为由原点和坐标轴定义的坐标系
boost::shared_ptr<COMPONENT::GeneralCoord> NodesToCoord(
    const MODEL::Model &model, const int &nid1, const int &nid2);
}  // namespace TOOL