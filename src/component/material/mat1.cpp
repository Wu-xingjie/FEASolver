#include "mat1.h"

namespace COMPONENT {

Mat1Data::Mat1Data(const Mat1Data &p) {
  if (p._E.is_initialized()) {
    _E = p._E;
  }
  if (p._G.is_initialized()) {
    _G = p._G;
  }
  if (p._NU.is_initialized()) {
    _NU = p._NU;
  }
}

MAT1::MAT1(const MAT1 &p) {
  _id = p._id;
  if (p._mat1_data._E.is_initialized()) {
    _mat1_data._E = p._mat1_data._E;
  }
  if (p._mat1_data._G.is_initialized()) {
    _mat1_data._G = p._mat1_data._G;
  }
  if (p._mat1_data._NU.is_initialized()) {
    _mat1_data._NU = p._mat1_data._NU;
  }
}

Mat1Data MAT1::GetMaterial() { return _mat1_data; }

void MAT1::SetComp(const file_data &datas) {
  if (!datas.front().at(2).empty()) {
    _id = boost::any_cast<int>(datas.front().at(1));
  }
  if (!datas.front().at(2).empty()) {
    _mat1_data._E = boost::any_cast<double>(datas.front().at(2));
  }
  if (!datas.front().at(3).empty()) {
    _mat1_data._NU = boost::any_cast<double>(datas.front().at(2));
  }
  if (!datas.front().at(4).empty()) {
    _mat1_data._G = boost::any_cast<double>(datas.front().at(2));
  }
}
}  // namespace COMPONENT