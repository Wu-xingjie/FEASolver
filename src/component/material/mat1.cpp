#include "mat1.h"

namespace COMPONENT {

MAT1::MAT1(const MAT1 &p) {
  _id = p._id;
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

std::map<std::string, boost::any> MAT1::GetMatDate() {
  std::map<std::string, boost::any> datas;
  if (_E.is_initialized()) {
    datas["E"] = _E.get();
  }
  if (_NU.is_initialized()) {
    datas["NU"] = _NU.get();
  }
  if (_G.is_initialized()) {
    datas["G"] = _G.get();
  }
  return datas;
}

void MAT1::SetComp(const file_data &datas) {
  if (!datas.front().at(1).empty()) {
    _id = boost::any_cast<int>(datas.front().at(1));
  }
  if (!datas.front().at(2).empty()) {
    try {
      if (datas.front().at(2).type() != typeid(std::string)) {
        _E = boost::any_cast<double>(datas.front().at(2));
      }
    } catch (const std::exception &e) {
    }
  }
  if (!datas.front().at(3).empty()) {
    try {
      if (datas.front().at(3).type() != typeid(std::string)) {
        _G = boost::any_cast<double>(datas.front().at(3));
      }
    } catch (const std::exception &e) {
    }
  }
  if (!datas.front().at(4).empty()) {
    try {
      if (datas.front().at(4).type() != typeid(std::string)) {
        _NU = boost::any_cast<double>(datas.front().at(4));
      }
    } catch (const std::exception &e) {
    }
  }
}
}  // namespace COMPONENT