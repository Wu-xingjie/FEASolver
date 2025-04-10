#pragma once
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

#include "id_manager/id_mananger.h"
#include "model_mapper/register.h"

namespace COMPONENT {
class PropertyBase : public CompBase {
 public:
  PropertyBase() { _id = TOOL::IdManager(TOOL::id_type::PROPERTY); }
  ~PropertyBase() = default;

  virtual void SetComp(const file_data &datas) = 0;
  virtual int GetID() = 0;
  virtual comp_type Type() = 0;

 protected:
  int _id;
};
}  // namespace COMPONENT