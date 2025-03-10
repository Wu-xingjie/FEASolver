#pragma once
#include "tool/id_manager/id_mananger.h"

namespace COMPONENT {
class MaterialBase {
 public:
  MaterialBase() { _id = TOOL::IdManager(TOOL::id_type::MATERIAL); }
  ~MaterialBase() = default;

 protected:
  int _id;
};
}  // namespace COMPONENT