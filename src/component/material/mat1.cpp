#include "mat1.h"

namespace COMPONENT {

Mat1Data::Mat1Data(const Mat1Data &p) {
  _E = p._E;
  _G = p._G;
  _NU = p._NU;
}

MAT1::MAT1(const MAT1 &p) {
  _id = p._id;
  _mat1_data._E = p._mat1_data._E;
  _mat1_data._G = p._mat1_data._G;
  _mat1_data._NU = p._mat1_data._NU;
}

void MAT1::SetMaterial(double E, double G, double NU) {
  _mat1_data._E = E;
  _mat1_data._G = G;
  _mat1_data._NU = NU;
}

Mat1Data MAT1::GetMaterial() { return _mat1_data; }

} // namespace COMPONENT