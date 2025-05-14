#include "mat1.h"

namespace COMPONENT {

// Mat1Data::Mat1Data(const Mat1Data &p) {
//   if (p._E.is_initialized()) {
//     _E = p._E;
//   }
//   if (p._G.is_initialized()) {
//     _G = p._G;
//   }
//   if (p._NU.is_initialized()) {
//     _NU = p._NU;
//   }
// }

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

boost::optional<double> MAT1::GetE() {
  boost::optional<double> result;
  if (_E.is_initialized()) {
    result = _E.get();
  }
  return result;
}

boost::optional<double> MAT1::GetNU() {
  boost::optional<double> result;
  if (_NU.is_initialized()) {
    result = _NU.get();
  }
  return result;
}

boost::optional<double> MAT1::GetG() {
  boost::optional<double> result;
  if (_G.is_initialized()) {
    result = _G.get();
  }
  return result;
}

// Mat1Data MAT1::GetMaterial() { return _mat1_data; }

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
} // namespace COMPONENT