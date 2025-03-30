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
  virtual ~CompBase() = default;
  virtual int GetID() = 0;
  virtual void SetComp(const file_data &datas) = 0;
};
} // namespace COMPONENT