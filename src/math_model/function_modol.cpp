#include "function_modol.h"

#include <boost/make_shared.hpp>
#include <regex>

namespace MATH {
FuncModel::FuncModel(const std::string& func) { Recrusion(func); }

void FuncModel::Recrusion(const std::string& func) {}

std::string FuncModel::AssignSubFunc(const std::string& func) {
  std::string result;
  
  return result;
}

bool FuncModel::IsSimplest(const std::string& func) {
  bool result = true;
  // 通过判断是否存在“*+*”来判断是否时最简函数表达式
  std::regex patten(R"((\d|\w)[+](\d|\w))");
  if (std::regex_search(func, patten)) {
    result = false;
  }
  return result;
}

bool FuncModel::NeedAssign(const std::string& func) {
  bool result = true;
  // 通过匹配是否存在子符号“)* or )* or )*(”来判断是否需要对函数进行拆分
  std::regex pattern(R"(\)\*|\*\(|\)\*\()");
  if (std::regex_match(func, pattern)) {
    result = false;
  }
  return result;
}

}  // namespace MATH