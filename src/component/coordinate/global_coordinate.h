#pragma once
#include "coordinate_base.h"

namespace COMPONENT {
class GlobalCoord : public CoordBase {
 public:
  GlobalCoord() {
    _id = 0;
    GeneralCoord temp;
    temp._coord_origin << 0, 0, 0;
    temp._vec1 << 1, 0, 0;
    temp._vec2 << 0, 1, 0;
    temp._vec3 << 0, 0, 1;
    temp._coord_flag = true;
    temp._dim_type = GeneralCoord::gen_coord_type::dim3;
    _general_coord = temp;
  }
  virtual ~GlobalCoord() = default;

  virtual void SetComp(const file_data &datas) override {
    std::cout << "[INFO]:该坐标系为全局坐标系" << std::endl;
  }
  virtual void TransToGeneralCoord(const MODEL::Model &model) override {
    std::cout << "[WARRING]>>>该坐标系为全局坐标,不需要进行转换" << std::endl;
  }
};
}  // namespace COMPONENT