#pragma once

#include "coordinate_base.h"
#include "id_manager/id_mananger.h"
namespace COMPONENT {
class CartesianCoord : public CoordBase {
public:
  CartesianCoord() : CoordBase() {}
  CartesianCoord(const vec_3 &origin, const vec_3 &vec1, const vec_3 &vec2,
                 const vec_3 &vec3, const bool &assign_id = true)
      : CoordBase(origin, vec1, vec2, vec3, assign_id) {}
  ~CartesianCoord() = default;
  // 拷贝构造
  CartesianCoord(const CartesianCoord &p);
  // get
  int getID();
  vec_3 getORIGIN();
  std::array<vec_3, 3> getVEC();
  // set
  void setORIGIN(const vec_3 &vec);
  void setVEC(const vec_3 &vec1, const vec_3 &vec2, const vec_3 &vec3);

protected:
  void setID(const int &id);
};
} // namespace COMPONENT