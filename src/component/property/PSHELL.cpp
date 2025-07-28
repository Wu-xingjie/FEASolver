#include "PSHELL.h"

namespace COMPONENT {
void PSHELL::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _mid1 = boost::any_cast<int>(card.at(2));
  _t = boost::any_cast<double>(card.at(3));
  if (card.at(2).type() == typeid(std::string)) {
    _mid2 = boost::any_cast<int>(card.at(2));
  } else {
    _mid2 = boost::any_cast<int>(card.at(4));
  }
  if (!card.at(5).empty()) {
    _inertia_ratio_of_bm = boost::any_cast<double>(card.at(5));
  }
}

std::map<std::string, boost::any> PSHELL::GetPropDate() {
  std::map<std::string, boost::any> datas;
  datas["_mid1"] = _mid1;
  datas["_t"] = _t;
  datas["_mid2"] = _mid2;
  datas["_inertia_ratio_of_bm"] = _inertia_ratio_of_bm;
  return datas;
}

} // namespace COMPONENT