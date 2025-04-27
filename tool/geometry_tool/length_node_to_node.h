#pragma once
#include "component/node/node.h"
#include "fea_model/fea_model.h"

namespace TOOL {
// 计算两节点之间的距离，可接受两节点不再同一坐标系下
double LenNode2Node(const MODEL::Model &model,
                    const boost::shared_ptr<COMPONENT::Node> n1,
                    const boost::shared_ptr<COMPONENT::Node> n2);
} // namespace TOOL