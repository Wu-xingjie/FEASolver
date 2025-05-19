#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
class BAR : public ElemBase {
 public:
  BAR() { _elem_type = ElemBase::elem_type::bar; }
  virtual ~BAR() = default;

 private:
  int _pid;
  int _G1;
  int _G2;
  std::array<double, 3> _origin_vec;
};
}  // namespace COMPONENT