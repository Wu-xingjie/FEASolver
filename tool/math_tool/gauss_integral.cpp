#include "gauss_integral.h"
#include <regex>
namespace TOOL {
std::vector<std::string> GaussIntegral(const int &num_r,
                                       const std::vector<std::string> &func) {
  std::vector<std::string> result;
  for (std::string elem : func) {
    // 1.检测函数各项系数合法性（是否包含除去符号*以外的其他符号）
    for(auto c : elem){
        
    }
    // 2.分离字符和数字
    // 3.权重因子*各项数字系数*积分点的n次方
  }
  return result;
}
} // namespace TOOL