#pragma once
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <map>

#include "model_mapper/register.h"

namespace COMPONENT {
class PropertyBase : public CompBase {
 public:
  PropertyBase() { _type = comp_type::prop; };
  ~PropertyBase() = default;
  
  virtual std::map<std::string, boost::any> GetPropDate() = 0;
};
}  // namespace COMPONENT