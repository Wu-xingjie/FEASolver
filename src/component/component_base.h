#pragma once
#include <array>
#include <boost/any.hpp>
#include <vector>
namespace COMPONENT {
typedef std::vector<std::array<boost::any, 10>> file_data;
// 所有元件都继承自该类
class CompBase {
 public:
  CompBase() = default;
  ~CompBase() = default;
};
}  // namespace COMPONENT