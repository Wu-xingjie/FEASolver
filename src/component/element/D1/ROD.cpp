#include "ROD.h"
#include "geometry_tool/length_node_to_node.h"
#include <eigen3/Eigen/Dense>

namespace COMPONENT {
void ROD::SetNode(const Node &g1, const Node &g2) {
  _G1 = g1;
  _G2 = g2;
}

void ROD::SetProperty(const PROD &p) { _property = p; }

void ROD::GenerateK() {
  double E = _property.GetProperty()._mat.GetMaterial()._E;
  double A = _property.GetProperty()._a;
  double L = TOOL::LenNode2Node(_G1, _G2);
  // clang-format off
  _loc_k << E * A / L, -E * A / L,
           -E * A / L, E * A / L;
  // clang-format on
}

Eigen::Matrix2d ROD::GetK() { return _loc_k; }

} // namespace COMPONENT