#include "PROD.h"

namespace COMPONENT {
ProdData::ProdData(const ProdData& p) {
  _a = p._a;
  _mat = p._mat;
}

PROD::PROD(const PROD& p) {
  _id = p._id;
  _prod_data._a = p._prod_data._a;
  _prod_data._mat = p._prod_data._mat;
  _prod_data._a = p._prod_data._a;
}

void PROD::SetProperty(double a, MAT1 mat) {
  _prod_data._a = a;
  _prod_data._mat = mat;
}

ProdData PROD::GetProperty() { return _prod_data; }

}  // namespace COMPONENT