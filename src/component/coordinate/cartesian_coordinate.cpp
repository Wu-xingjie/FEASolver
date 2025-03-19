#include "cartesian_coordinate.h"

namespace COMPONENT {

int CartesianCoord::getID() { return _coord_id; }

vec_3 CartesianCoord::getORIGIN() { return _coord_origin; }

std::array<vec_3, 3> CartesianCoord::getVEC() {
  std::array<vec_3, 3> axi_vec = {_vec1, _vec2, _vec3};
  return axi_vec;
}

void CartesianCoord::setORIGIN(const vec_3 &vec) { _coord_origin = vec; }

void CartesianCoord::setVEC(const vec_3 &vec1, const vec_3 &vec2,
                            const vec_3 &vec3) {
  _vec1 = vec1;
  _vec2 = vec2;
  _vec3 = vec3;
}

// void CartesianCoord::setID(const int &id) { _coord_id = id; }

CartesianCoord::CartesianCoord(const CartesianCoord &p) {
  _coord_id = p._coord_id;
  _coord_origin = p._coord_origin;
  _vec1 = p._vec1;
  _vec2 = p._vec2;
  _vec3 = p._vec3;
}

} // namespace COMPONENT