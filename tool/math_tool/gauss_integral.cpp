#include "gauss_integral.h"

#include <boost/make_shared.hpp>
#include <regex>

#include "func_expr/func_cal.h"
namespace TOOL {
boost::shared_ptr<double> GaussIntegral(const std::string& func,
                                        const double& idx_up,
                                        const double& idx_low,
                                        const int& num_r,
                                        const int& num_integal) {
  auto result = boost::make_shared<double>();
  // 获取采样点和权值
  // 
  return result;
}
}  // namespace TOOL