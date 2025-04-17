#pragma once
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

#include "model_mapper/register.h"

namespace COMPONENT {
class PropertyBase : public CompBase {
 public:
  PropertyBase() { _type = comp_type::prop; };
  ~PropertyBase() = default;
  // 获取截面面积
  virtual double GetCrossArea() = 0;
  virtual int GetMatId() = 0;
};
}  // namespace COMPONENT