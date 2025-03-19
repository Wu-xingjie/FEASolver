#pragma once
#include "src_inc_dir.h"

namespace COMPONENT {
class MaterialBase {
 public:
  MaterialBase() { _id = TOOL::IdManager(TOOL::id_type::MATERIAL); }
  ~MaterialBase() = default;

 protected:
  int _id;
};
}  // namespace COMPONENT