#include "PSOLID.h"

namespace COMPONENT {

void PSOLID::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _mid = boost::any_cast<int>(card.at(2));
}

std::map<std::string, boost::any> PSOLID::GetPropDate() {
  std::map<std::string, boost::any> datas;
  datas["_mid"] = _mid;
  return datas;
}

} // namespace COMPONENT