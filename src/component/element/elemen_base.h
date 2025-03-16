#pragma once
#include "id_manager/id_mananger.h"

namespace COMPONENT {
class ElemBase {
public:
  ElemBase() { _id = TOOL::IdManager(TOOL::id_type::ELEMENT); };
  ~ElemBase() = default;

protected:
  int _id;
};
} // namespace COMPONENT