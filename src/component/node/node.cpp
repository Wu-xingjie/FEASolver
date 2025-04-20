#include "node.h"

namespace COMPONENT {
Node::Node(const Node &n) {
  _coord = n._coord;
  _x = n._x;
  _y = n._y;
  _z = n._z;
}

int Node::GetCoord() const { return _coord; }

vec_3 Node::get_location() const {
  vec_3 loc(_x, _y, _z);
  return loc;
}

void Node::SetComp(const file_data &datas) {
  try {
    if (datas.empty()) {
      throw "节点设置时输入数据为空！";
    }
    auto card = datas.front();

    _id = boost::any_cast<int>(card.at(1));
    if (card.at(2).type() == typeid(std::string)) {
      _coord = 0;
    } else {
      _coord = boost::any_cast<int>(card.at(2));
    }
    _x = boost::any_cast<double>(card.at(3));
    _y = boost::any_cast<double>(card.at(4));
    _z = boost::any_cast<double>(card.at(5));
  } catch (const char *e) {
    std::cerr << e << '\n';
  }
}

} // namespace COMPONENT