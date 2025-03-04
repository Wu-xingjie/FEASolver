#include "cartesian_coordinate.h"

int COORDINATE::CartesianCoord::getID() { return _coord_id; }

COORDINATE::arr3 COORDINATE::CartesianCoord::getORIGIN() {
  return _coord_origin;
}

std::array<COORDINATE::arr3, 3> COORDINATE::CartesianCoord::getVEC() {
  std::array<COORDINATE::arr3, 3> axi_vec = {_vec1, _vec2, _vec3};
  return axi_vec;
}