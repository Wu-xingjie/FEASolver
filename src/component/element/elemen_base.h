#pragma once
#include "component_base.h"
#include "model_mapper/register.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

namespace COMPONENT {
class ElemBase : public CompBase {
public:
  ElemBase() { _type = comp_type::element; };
  ~ElemBase() = default;

protected:
  int _id;
};
} // namespace COMPONENT