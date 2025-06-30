#include "func_cal.h"
#include "exprtk.hpp"
#include <stdexcept>
namespace TOOL {
double FuncCal(const std::string &func, double x, double y) {
  //   定义符号表
  exprtk::symbol_table<double> sym_talbe;
  //   增加变量
  sym_talbe.add_variable("x", x);
  sym_talbe.add_variable("y", y);

  //   创建表达式
  exprtk::expression<double> expr;
  expr.register_symbol_table(sym_talbe); //   注册变量表

  //   创建解析器
  exprtk::parser<double> parser;
  if (!parser.compile(func, expr)) {
    throw std::runtime_error("[ERROR]:func(FuncCal)>>>表达式解析失败!");
  }
  return expr.value();
}
} // namespace TOOL