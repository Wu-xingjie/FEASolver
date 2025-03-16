#pragma once
#include "component/node/node.h"

namespace TOOL {
// 计算两节点之间的距离，可接受两节点不再同一坐标系下
double LenNode2Node(const COMPONENT::Node &n1, const COMPONENT::Node &n2);
} // namespace TOOL