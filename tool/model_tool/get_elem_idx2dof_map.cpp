#include "get_elem_idx2dof_map.h"
#include <algorithm>
#include <array>
#include <boost/lexical_cast.hpp>
namespace TOOL {
std::map<std::array<int, 2>, std::array<std::string, 2>>
ElemIdx2Dof(COMPONENT::ElemBase &elem_base) {
  std::map<std::array<int, 2>, std::array<std::string, 2>> result;
  auto nodes = elem_base.GetNodes();
  std::sort(nodes.begin(), nodes.end());
  // 创建单刚矩阵索引到自由度的map
  int num = 0;
  std::map<int, std::string> temp_map;
  for (int &n : nodes) {
    temp_map[num] = std::to_string(n) + "_vx";
    temp_map[num + 1] = std::to_string(n) + "_vy";
    temp_map[num + 2] = std::to_string(n) + "_vz";
    temp_map[num + 3] = std::to_string(n) + "_rx";
    temp_map[num + 4] = std::to_string(n) + "_ry";
    temp_map[num + 5] = std::to_string(n) + "_rz";
    num += 6;
  }
  // 生成刚度矩阵自由度坐标到
  int k_size = 6 * nodes.size();
  for (int c = 0; c < k_size; c++) {
    for (int r = 0; r < c + 1; r++) {
      std::array<std::string, 2> dof_pair;
      dof_pair[0] = temp_map.at(r);
      dof_pair[1] = temp_map.at(c);
      result[{r, c}] = dof_pair;
    }
  }
  return result;
}
} // namespace TOOL