#include "id_mananger.h"

static int NodeID = 0;
static int ELEMENTID = 0;
static int LOADID = 0;
static int COORDINATEID = 0;

int TOOL::IdManager(const TOOL::id_type &type) {
  try {
    int num = 0;
    switch (type) {
      case TOOL::id_type::NODE:
        NodeID += 1;
        num = NodeID;
        break;
      case TOOL::id_type::LOAD:
        LOADID += 1;
        num = LOADID;
        break;
      case TOOL::id_type::COORDINATE:
        COORDINATEID += 1;
        num = COORDINATEID;
        break;
      case TOOL::id_type::ELEMENT:
        ELEMENTID += 1;
        num = ELEMENTID;
        break;
      default:
        break;
    }
    return num;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}