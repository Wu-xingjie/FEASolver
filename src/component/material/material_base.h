#pragma once
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

#include "component/component_base.h"
#include "model_mapper/register.h"
namespace COMPONENT {
class MaterialBase : public CompBase {
public:
  MaterialBase() { _type = comp_type::mat; };
  virtual ~MaterialBase() = default;


};
} // namespace COMPONENT