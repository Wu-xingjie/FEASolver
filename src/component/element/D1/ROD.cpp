#include "ROD.h"
#include "geometry_tool/length_node_to_node.h"

namespace COMPONENT {
void ROD::SetNode(const int &g1, const int &g2) {
  _G1 = g1;
  _G2 = g2;
}

void ROD::SetProperty(const int &p) { _property = p; }

// void ROD::GenerateK() {
//   double E = _property.GetProperty()._mat.GetMaterial()._E;
//   double A = _property.GetProperty()._a;
//   double L = TOOL::LenNode2Node(_G1, _G2);
//   // clang-format off
//   _loc_k << E * A / L, -E * A / L,
//            -E * A / L, E * A / L;
//   // clang-format on
// }

Eigen::Matrix2d ROD::GetK() { return _loc_k; }

} // namespace COMPONENT