#include <iostream>
#include <set>
#include <stdexcept>

namespace TOOL {
// id类型
enum id_type {
  NODE,
  ELEMENT,
  LOAD,
  COORDINATE,
};

static int NodeID = 0;
static int ELEMENTID = 0;
static int LOADID = 0;
static int COORDINATEID = 0;

int IdManager(const id_type& type) {
  try {
    switch (type) {
      case id_type::NODE:
        NodeID += 1;
        return NodeID;
        break;
      case id_type::LOAD:
        LOADID += 1;
        return LOADID;
        break;
      case id_type::COORDINATE:
        COORDINATEID += 1;
        return COORDINATEID;
        break;
      case id_type::ELEMENT:
        ELEMENTID += 1;
        return ELEMENTID;
        break;
      default:
        break;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}
}  // namespace TOOL