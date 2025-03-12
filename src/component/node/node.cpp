#include "node.h"

namespace COMPONENT {
Node::Node(const Node &n) {
  _coord = n._coord;
  _x = n._x;
  _y = n._y;
  _z = n._z;
}

CartesianCoord Node::GetCoord() { return _coord; }

arr3 Node::get_location() {
  arr3 loc{_x, _y, _z};
  return loc;
}

void Node::set_node(const double &x, const double &y, const double &z,
                    const CartesianCoord &coord) {
  _coord = coord;
  _x = x;
  _y = y;
  _z = z;
}

}  // namespace COMPONENT