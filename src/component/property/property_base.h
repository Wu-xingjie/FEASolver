#pragma once
#include "tool/id_manager/id_mananger.h"

namespace COMPONENT {
class PropertyBase {
 public:
  PropertyBase() { _id = TOOL::IdManager(TOOL::id_type::PROPERTY); }
  ~PropertyBase() = default;

 protected:
  int _id;
};
}  // namespace COMPONENT