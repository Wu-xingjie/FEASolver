#include "cartesian_coordinate.h"

namespace COMPONENT {

int CartesianCoord::GetID() { return _id; }

vec_3 CartesianCoord::getORIGIN() { return _coord_origin; }

std::array<vec_3, 3> CartesianCoord::getVEC() {
  std::array<vec_3, 3> axi_vec = {_vec1, _vec2, _vec3};
  return axi_vec;
}

void CartesianCoord::SetComp(const file_data &datas) {
  std::cout << "暂时不处理坐标系问题" << std::endl;
}

CartesianCoord::CartesianCoord(const CartesianCoord &p) {
  _id = p._id;
  _coord_origin = p._coord_origin;
  _vec1 = p._vec1;
  _vec2 = p._vec2;
  _vec3 = p._vec3;
}

} // namespace COMPONENT