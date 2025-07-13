#include "area_func_expr.h"

namespace TOOL {
std::string AreaFuncExpr(const Eigen::Vector2d &p_1,
                         const Eigen::Vector2d &p_2) {
  return "((" + std::to_string(p_1(0)) + "*" + std::to_string(p_2(1)) + "-" +
         std::to_string(p_1(1)) + "*" + std::to_string(p_2(0)) + ")-x*(" +
         std::to_string(p_2(1)) + "-" + std::to_string(p_1(1)) + ")+y*(" +
         std::to_string(p_2(0)) + std::to_string(p_1(0)) + "))";
}
} // namespace TOOL