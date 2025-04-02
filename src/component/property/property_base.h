#pragma once
#include "id_manager/id_mananger.h"
#include <eigen3/Eigen/Dense>
#include "model_mapper/register.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace COMPONENT {
class PropertyBase {
 public:
  PropertyBase() { _id = TOOL::IdManager(TOOL::id_type::PROPERTY); }
  ~PropertyBase() = default;

 protected:
  int _id;
};
}  // namespace COMPONENT