#pragma once
#include <boost/optional.hpp>
#include <iostream>

namespace TOOL {
template <typename T>
void GetOptValue(const boost::optional<T>& p, T& target) {
  try {
    if (p) {
      if (p.has_value()) {
        target = p.get();
      } else {
        throw "optional指针未初始化";
      }
    } else {
      throw "无效optional指针";
    }
  }
}

}  // namespace TOOL