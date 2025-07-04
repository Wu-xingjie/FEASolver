#pragma once
#include <string>
namespace TOOL {
// 基于开源库ExprTk的数学函数求解器
double FuncCal(const std::string &func, double x = 0.0, double y = 0.0,
               double z = 0.0);
}  // namespace TOOL