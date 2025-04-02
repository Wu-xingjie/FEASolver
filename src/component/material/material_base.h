#pragma once
#include "component/component_base.h"
#include "id_manager/id_mananger.h"
#include <eigen3/Eigen/Dense>
#include "model_mapper/register.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace COMPONENT {
class MaterialBase : public CompBase {
public:
  MaterialBase() { _id = TOOL::IdManager(TOOL::id_type::MATERIAL); }
  virtual ~MaterialBase() = default;

  virtual void SetComp(const file_data &datas) = 0;
  virtual int GetID() = 0;

protected:
  int _id;
};
} // namespace COMPONENT