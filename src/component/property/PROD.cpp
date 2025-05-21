#include "PROD.h"

namespace COMPONENT {

PROD::PROD(const PROD &p) {
  _id = p._id;
  _a = p._a;
  _mid = p._mid;
}

void PROD::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _mid = boost::any_cast<int>(card.at(2));
  _a = boost::any_cast<double>(card.at(3));
}

std::map<std::string, boost::any> PROD::GetPropDate() {
  std::map<std::string, boost::any> datas;
  datas["mid"] = _mid;
  datas["a"] = _a;
  return datas;
}

}  // namespace COMPONENT