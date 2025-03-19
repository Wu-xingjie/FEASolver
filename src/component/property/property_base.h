#pragma once
#include "src_inc_dir.h"

namespace COMPONENT {
class PropertyBase {
 public:
  PropertyBase() { _id = TOOL::IdManager(TOOL::id_type::PROPERTY); }
  ~PropertyBase() = default;

 protected:
  int _id;
};
}  // namespace COMPONENT