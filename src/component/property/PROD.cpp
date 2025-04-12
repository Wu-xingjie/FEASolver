#include "PROD.h"

namespace COMPONENT {
ProdData::ProdData(const ProdData &p) {
  _a = p._a;
  _mid = p._mid;
}

PROD::PROD(const PROD &p) {
  _id = p._id;
  _prod_data._a = p._prod_data._a;
  _prod_data._mid = p._prod_data._mid;
}

void PROD::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _prod_data._mid = boost::any_cast<int>(card.at(2));
  _prod_data._a = boost::any_cast<int>(card.at(3));
}

ProdData PROD::GetProperty() { return _prod_data; }

} // namespace COMPONENT