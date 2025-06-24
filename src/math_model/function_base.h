#pragma once
#include <array>
#include <regex>
#include <string>
namespace MATH {

class MathBase {
 public:
  MathBase() = default;
  virtual ~MathBase() = default;

  virtual void FuncParser(const std::string& func) = 0;
  virtual std::string Calculate(const std::array<std::string, 3>& xyz) = 0;
};



}  // namespace MATH