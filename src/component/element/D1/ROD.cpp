#include "ROD.h"

namespace COMPONENT {
void ROD::SetNode(const Node &g1, const Node &g2) {
  _G1 = g1;
  _G2 = g2;
}

void ROD::SetProperty(const PROD &p) { _property = p; }

}  // namespace COMPONENT