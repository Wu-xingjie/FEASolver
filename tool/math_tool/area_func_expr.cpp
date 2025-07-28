/*
 * @Author: wxj wxj122350@126.com
 * @Date: 2025-07-13 22:02:07
 * @LastEditors: wxj wxj122350@126.com
 * @LastEditTime: 2025-07-28 16:44:45
 * @FilePath: /FEASolver/tool/math_tool/area_func_expr.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "area_func_expr.h"

namespace TOOL {
std::string AreaFuncExpr(const Eigen::Vector2d &p_1,
                         const Eigen::Vector2d &p_2) {
  return "((" + std::to_string(p_1(0)) + "*" + std::to_string(p_2(1)) + "-" +
         std::to_string(p_1(1)) + "*" + std::to_string(p_2(0)) + ")-x*(" +
         std::to_string(p_2(1)) + "-" + std::to_string(p_1(1)) + ")+y*(" +
         std::to_string(p_2(0)) + "-" + std::to_string(p_1(0)) + "))";
}
} // namespace TOOL