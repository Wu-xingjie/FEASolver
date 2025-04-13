#pragma once
#include "component/component_base.h"
#include "model_mapper/register.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

namespace COMPONENT {
class ElemBase : public CompBase {
public:
  ElemBase() { _type = comp_type::element; };
  ~ElemBase() = default;


  // virtual void GenerateK(const MODEL::Model &model) = 0;

protected:
  int _id;
};
} // namespace COMPONENT