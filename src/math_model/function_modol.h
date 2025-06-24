#pragma once
#include <boost/shared_ptr.hpp>
#include <vector>

#include "exponential_func.h"
namespace MATH {
class FuncModel {
 public:
  FuncModel() = default;
  ~FuncModel() = default;

  
  
 private:
  std::vector<boost::shared_ptr<ExpoFunc>> _expos;
};
}  // namespace MATH