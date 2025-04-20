#include "cartesian_by_node.h"

namespace COMPONENT {

int CartesianCoord::GetID() { return _id; }

vec_3 CartesianCoord::getORIGIN() { return _general_coord._coord_origin; }

std::array<vec_3, 3> CartesianCoord::getVEC() {
  std::array<vec_3, 3> axi_vec = {_general_coord._vec1, _general_coord._vec2,
                                  _general_coord._vec3};
  return axi_vec;
}

void CartesianCoord::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _N1 = boost::any_cast<int>(card.at(2));
  _N2 = boost::any_cast<int>(card.at(3));
  _N3 = boost::any_cast<int>(card.at(4));
}

CartesianCoord::CartesianCoord(const CartesianCoord &p) {
  _id = p._id;
  _general_coord._coord_origin = p._general_coord._coord_origin;
  _general_coord._vec1 = p._general_coord._vec1;
  _general_coord._vec2 = p._general_coord._vec2;
  _general_coord._vec3 = p._general_coord._vec3;
}

GeneralCoord CartesianCoord::GetGeneralCoord() {
  GeneralCoord result;
  // TODO: 转换该坐标系为一般坐标系
  return result;
}

} // namespace COMPONENT