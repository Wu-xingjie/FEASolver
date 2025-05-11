#include "single_point_constrained.h"

namespace COMPONENT {

int GetElemNumPerCard(const std::array<boost::any, 10> &card) {
  int data_num = 0;
  for (int i = 9; i >= 0; i--) {
    if (card.at(i).empty()) {
      continue;
    } else {
      data_num = i;
      break;
    }
  }
  return data_num + 1;
}

void SinglePointConstrains::SetComp(const file_data &datas) {
  // TODO：暂时只处理一个卡片只有一行的单点约束集
  //   获取单点约束个数
  int num_data = GetElemNumPerCard(datas.front()) - 2;
  int num_spc = 0;
  if (num_data % 3 == 0) {
    num_spc = num_data / 3;
  } else {
    num_spc = 1 + num_data / 3;
  }
  // 给单点约束集赋值
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  for (int i = 0; i < num_spc; i++) {
    SPC temp_spc;
    temp_spc._node = boost::any_cast<int>(card.at(2 + 3 * i));
    temp_spc._component = std::to_string(boost::any_cast<int>(card.at(3 + 3 * i)));
    temp_spc._value = boost::any_cast<double>(card.at(4 + 3 * i));
    _spc.push_back(temp_spc);
  }
}
} // namespace COMPONENT