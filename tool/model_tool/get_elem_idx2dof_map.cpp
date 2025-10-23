#include "get_elem_idx2dof_map.h"
#include <algorithm>
#include <array>
#include <boost/lexical_cast.hpp>
namespace TOOL {
std::map<int, std::string> ElemIdx2Dof(COMPONENT::ElemBase &elem_base) {
  std::map<int, std::string> result;
  auto nodes = elem_base.GetNodes();
  std::sort(nodes.begin(), nodes.end());
  int num = 0;
  for (int &n : nodes) {
    result[num] = std::to_string(n) + "_vx";
    result[num + 1] = std::to_string(n) + "_vy";
    result[num + 2] = std::to_string(n) + "_vz";
    result[num + 3] = std::to_string(n) + "_rx";
    result[num + 4] = std::to_string(n) + "_ry";
    result[num + 5] = std::to_string(n) + "_rz";
    num += 6;
  }
  return result;
}
} // namespace TOOL