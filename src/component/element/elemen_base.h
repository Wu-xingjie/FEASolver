#pragma once
#include "id_manager/id_mananger.h"
#include <eigen3/Eigen/Dense>
#include "model_mapper/register.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace COMPONENT {
class ElemBase {
public:
  ElemBase() { _id = TOOL::IdManager(TOOL::id_type::ELEMENT); };
  ~ElemBase() = default;

protected:
  int _id;
};
} // namespace COMPONENT