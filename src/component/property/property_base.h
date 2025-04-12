#pragma once
#include "model_mapper/register.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

namespace COMPONENT {
class PropertyBase : public CompBase {
public:
  PropertyBase() = default;
  ~PropertyBase() = default;

  virtual void SetComp(const file_data &datas) = 0;
  virtual int GetID() = 0;
  virtual comp_type Type() = 0;

protected:
  int _id;
};
} // namespace COMPONENT