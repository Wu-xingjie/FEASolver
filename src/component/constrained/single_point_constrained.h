#pragma once
#include "constrained_base.h"
namespace COMPONENT {
struct SPC {
  int _node;
  int _component;
  double _value{0.0};
};

class SinglePointConstrains : public ConstrainBase {
public:
  SinglePointConstrains() = default;
  virtual ~SinglePointConstrains() = default;

  virtual void SetComp(const file_data &datas) override;

private:
  std::vector<SPC> _spc;
};
} // namespace COMPONENT