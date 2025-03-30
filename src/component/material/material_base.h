#pragma once
#include "component/component_base.h"
#include "src_inc_dir.h"
namespace COMPONENT {
class MaterialBase : public CompBase {
public:
  MaterialBase() { _id = TOOL::IdManager(TOOL::id_type::MATERIAL); }
  virtual ~MaterialBase() = default;

  virtual void SetComp(const file_data &datas) = 0;
  virtual int GetID() = 0;

protected:
  int _id;
};
} // namespace COMPONENT