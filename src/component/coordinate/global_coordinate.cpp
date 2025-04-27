#include "global_coordinate.h"

namespace COMPONENT {
// GlobalCoord::GlobalCoord() {
//   _id = 0;
//   GeneralCoord temp;
//   temp._coord_origin << 0, 0, 0;
//   temp._vec1 << 1, 0, 0;
//   temp._vec2 << 0, 1, 0;
//   temp._vec3 << 0, 0, 1;
//   temp._coord_flag = true;
//   _general_coord = temp;
// }
void GlobalCoord::SetComp(const file_data &datas) {
  std::cout << "[INFO]:该坐标系为全局坐标系" << std::endl;
}
} // namespace COMPONENT