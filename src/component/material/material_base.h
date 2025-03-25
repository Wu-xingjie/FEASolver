#pragma once
#include "component_base.h"
#include "src_inc_dir.h"
namespace COMPONENT {
class MaterialBase : public CompBase {
 public:
  MaterialBase() { _id = TOOL::IdManager(TOOL::id_type::MATERIAL); }
  ~MaterialBase() = default;
  
  virtual void GenerateComponent(const file_data& src_data) = 0;

 protected:
  int _id;
};
}  // namespace COMPONENT