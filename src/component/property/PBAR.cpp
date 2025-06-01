#include "PBAR.h"

namespace COMPONENT {
void PBAR::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _mid = boost::any_cast<int>(card.at(2));
  _a = boost::any_cast<double>(card.at(3));
  _I1 = boost::any_cast<double>(card.at(4));
  _I2 = boost::any_cast<double>(card.at(5));
  _J = boost::any_cast<double>(card.at(6));
}

std::map<std::string, boost::any> PBAR::GetPropDate() {
  std::map<std::string, boost::any> datas;
  datas["mid"] = _mid;
  datas["a"] = _a;
  datas["I1"] = _I1;
  datas["I2"] = _I2;
  datas["J"] = _J;
  return datas;
}
}  // namespace COMPONENT