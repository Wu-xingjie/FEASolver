#include "cartesian_coordinate.h"

namespace COMPONENT {

int CartesianCoord::getID() { return _coord_id; }

arr3 CartesianCoord::getORIGIN() { return _coord_origin; }

std::array<arr3, 3> CartesianCoord::getVEC() {
  std::array<arr3, 3> axi_vec = {_vec1, _vec2, _vec3};
  return axi_vec;
}

void CartesianCoord::setORIGIN(const arr3 &vec) { _coord_origin = vec; }

void CartesianCoord::setVEC(const arr3 &vec1, const arr3 &vec2,
                            const arr3 &vec3) {
  _vec1 = vec1;
  _vec2 = vec2;
  _vec3 = vec3;
}

void CartesianCoord::setID(const int &id) { _coord_id = id; }

CartesianCoord::CartesianCoord(const CartesianCoord &p) {
  // int id = p._coord_id;
  // arr3 origin = p._coord_origin;
  // arr3 vec1 = p._vec1;
  // arr3 vec2 = p._vec2;
  // arr3 vec3 = p._vec3;
  // CartesianCoord temp(origin, vec1, vec2, vec3, false);
  // temp.setID(id);
  // std::cout << "temp.id = " << temp.getID() << std::endl;
  _coord_id = p._coord_id;
  _coord_origin = p._coord_origin;
  _vec1 = p._vec1;
  _vec2 = p._vec2;
  _vec3 = p._vec3;
}

} // namespace COMPONENT