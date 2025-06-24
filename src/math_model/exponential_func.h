#include <map>

#include "function_base.h"
namespace MATH {
// 指数型函数模型
class ExpoFunc : public MathBase {
 public:
  ExpoFunc() = default;
  ~ExpoFunc() = default;

  void FuncParser(const std::string& func) override;
  std::string Calculate(const std::array<std::string, 3>& xyz) override;

 private:
  double _coff;                             // 系数
  std::pair<std::string, double> _x_power;  // 变量x及其幂
  std::pair<std::string, double> _y_power;  // 变量y及其幂
  std::pair<std::string, double> _z_power;  // 变量z及其幂
};
}  // namespace MATH