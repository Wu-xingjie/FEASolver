#pragma once
#include "id_manager/id_mananger.h"

namespace COMPONENT {
class LoadBase {
 public:
  LoadBase() { _id = TOOL::IdManager(TOOL::LOAD); }
  ~LoadBase() = default;

  virtual int GetID() = 0;

 protected:
  int _id;
};
}  // namespace COMPONENT