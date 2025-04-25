#include "coord_to_coord.h"

namespace TOOL {
  Eigen::Matrix3d CoordToCoord(
    const boost::shared_ptr<COMPONENT::CoordBase> base,
    const boost::shared_ptr<COMPONENT::CoordBase> obj) {
  try {
    // 获取两坐标系的一般坐标系
    auto gen_base = base->GetGeneralCoord();
    auto gen_obj = obj->GetGeneralCoord();
    
    

  } catch (const char* e) {
    std::cout << e << '\n';
  }
}
}  // namespace TOOL