#pragma once
#include <cmath>

#include "component/node/node.h"

namespace TOOL {
// 返回两个节点之间的距离
double LenN2N(const COMPONENT::Node& n1, const COMPONENT::Node& n2) {
  double l;
  COMPONENT::arr3 loc_n1 = n1.get_location();
  COMPONENT::arr3 loc_n2 = n2.get_location();
  // 两节点在同一坐标系下的情况
  if (n1.GetCoord() == n2.GetCoord()) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
      sum += std::pow(loc_n1.at(i) - loc_n2.at(i), 2);
    }
    l = std::sqrt(sum);
  } else {  // 两节点在不同坐标系下的情况。同时转换到全局坐标系，再计算距离
  }
}
}  // namespace TOOL