#pragma once

#include "coordinate_base.h"
#include "id_manager/id_mananger.h"
namespace COORDINATE {
class CartesianCoord : public CoordBase {
 public:
  CartesianCoord(const arr3& origin, const arr3& vec1, const arr3& vec2,
                 const arr3& vec3)
      : CoordBase(origin, vec1, vec2, vec3) {}
  ~CartesianCoord() = default;

  int getID();
  arr3 getORIGIN();
  std::array<arr3, 3> getVEC();
};
}  // namespace COORDINATE