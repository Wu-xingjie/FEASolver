#pragma once
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <eigen3/Eigen/Dense>

#include "component/component_base.h"
#include "model_mapper/register.h"
namespace COMPONENT {
class MaterialBase : public CompBase {
 public:
  MaterialBase() { _type = comp_type::mat; };
  virtual ~MaterialBase() = default;

  virtual std::map<std::string, boost::any> GetMatDate() = 0;
};
}  // namespace COMPONENT