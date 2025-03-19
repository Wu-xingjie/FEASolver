#include "PROD.h"

namespace COMPONENT {
ProdData::ProdData(const ProdData& p) {
  _a = p._a;
  _mid = p._mid;
}

PROD::PROD(const PROD& p) {
  _id = p._id;
  _prod_data._a = p._prod_data._a;
  _prod_data._mid = p._prod_data._mid;
  _prod_data._a = p._prod_data._a;
}

void PROD::SetProperty(double a, int mat) {
  _prod_data._a = a;
  _prod_data._mid = mat;
}

ProdData PROD::GetProperty() { return _prod_data; }

}  // namespace COMPONENT