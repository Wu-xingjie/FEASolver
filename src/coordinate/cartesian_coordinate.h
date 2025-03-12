#pragma once

#include "coordinate_base.h"
#include "id_manager/id_mananger.h"
namespace COMPONENT {
class CartesianCoord : public CoordBase {
public:
  CartesianCoord() : CoordBase() {}
  CartesianCoord(const arr3 &origin, const arr3 &vec1, const arr3 &vec2,
                 const arr3 &vec3, const bool &assign_id = true)
      : CoordBase(origin, vec1, vec2, vec3, assign_id) {}
  ~CartesianCoord() = default;
  // 拷贝构造
  CartesianCoord(const CartesianCoord &p);
  // get
  int getID();
  arr3 getORIGIN();
  std::array<arr3, 3> getVEC();
  // set
  void setORIGIN(const arr3 &vec);
  void setVEC(const arr3 &vec1, const arr3 &vec2, const arr3 &vec3);

protected:
  void setID(const int &id);
};
} // namespace COMPONENT