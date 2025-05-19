#include "PBAR.h"

namespace COMPONENT {
void PBAR::SetComp(const file_data &datas) {
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _mid = boost::any_cast<int>(card.at(2));
  _a = boost::any_cast<double>(card.at(3));
  _I1 = boost::any_cast<double>(card.at(3));
  _I2 = boost::any_cast<double>(card.at(3));
  _J = boost::any_cast<double>(card.at(3));
}
}  // namespace COMPONENT