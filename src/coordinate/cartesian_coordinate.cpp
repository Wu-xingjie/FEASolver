#include "cartesian_coordinate.h"

int COORDINATE::CartesianCoord::getID() { return _coord_id; }

COORDINATE::arr3 COORDINATE::CartesianCoord::getORIGIN() {
  return _coord_origin;
}

std::array<COORDINATE::arr3, 3> COORDINATE::CartesianCoord::getVEC() {
  std::array<COORDINATE::arr3, 3> axi_vec = {_vec1, _vec2, _vec3};
  return axi_vec;
}

void COORDINATE::CartesianCoord::setORIGIN(const COORDINATE::arr3& vec) {
  _coord_origin = vec;
}

void COORDINATE::CartesianCoord::setVEC(const COORDINATE::arr3& vec1,
                                        const COORDINATE::arr3& vec2,
                                        const COORDINATE::arr3& vec3) {
  _vec1 = vec1;
  _vec2 = vec2;
  _vec3 = vec3;
}