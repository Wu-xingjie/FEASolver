#include "node.h"

namespace NODE {
Node::Node(const Node &n) {
  _cid = n._cid;
  _x = n._x;
  _y = n._y;
  _z = n._z;
}

arr3 Node::get_location() {
  arr3 loc{_x, _y, _z};
  return loc;
}

void Node::set_node(const double &x, const double &y, const double &z,
                    const int &coord) {
  _cid = coord;
  _x = x;
  _y = y;
  _z = z;
}

} // namespace NODE