#include "function_modol.h"

#include <boost/make_shared.hpp>
#include <regex>

// 解析元函数的正则表达式：((\d(\.\d)?\*[xyz](\^\d)?)|((\d)(\.\d)?)|([xyz](\^\d)?))
// 4*x^3*(5*y^2+6*y*(2*x+3*x))+7*(8.3+3*x^2*(3+x))+4.3+x^5

namespace MATH {
FuncModel::FuncModel(const std::string &func) { Recrusion(func); }

void FuncModel::Recrusion(const std::string &func) {}

std::string FuncModel::AssignSubFunc(const std::string &func) {
  std::string result;
  
  return result;
}

std::vector<std::string> FuncModel::SplitFunc(const std::string &func) {
  // 通过比较”+“或”-“左侧字符串中”(“和”)“个数是否相等来判断是否在当前加号或减号出切断
  std::vector<std::string> result;
  int count_left = 0;
  int count_right = 0;
  for (int i = 0; i < func.size(); i++) {
    std::string sub_func;
    if (func.at(i) == '(') {
      count_left += 1;
    }
    if (func.at(i) == ')') {
      count_right += 1;
    }
    if (func.at(i) == '+' or func.at(i) == '-') {
      if (count_left == count_right) {
        result.push_back(sub_func);
        sub_func.clear();
      }
    }
    sub_func.push_back(func.at(i));
  }

  return result;
}

bool FuncModel::IsSimplest(const std::string &func) {
  bool result = true;
  // 通过判断是否存在“*+*”来判断是否时最简函数表达式
  std::regex patten(R"((\d|\w)[+-](\d|\w))");
  if (std::regex_search(func, patten)) {
    result = false;
  }
  return result;
}

bool FuncModel::NeedAssign(const std::string &func) {
  bool result = true;
  // 通过匹配是否存在子符号“)* or )* or )*(”来判断是否需要对函数进行拆分
  std::regex pattern(R"(\)\*|\*\(|\)\*\()");
  if (std::regex_match(func, pattern)) {
    result = false;
  }
  return result;
}

} // namespace MATH