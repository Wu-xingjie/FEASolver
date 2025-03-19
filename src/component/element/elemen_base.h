#pragma once
#include "src_inc_dir.h"

namespace COMPONENT {
class ElemBase {
public:
  ElemBase() { _id = TOOL::IdManager(TOOL::id_type::ELEMENT); };
  ~ElemBase() = default;

protected:
  int _id;
};
} // namespace COMPONENT