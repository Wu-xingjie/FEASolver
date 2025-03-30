#pragma once

#include "component/material/mat1.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class Mat1Factory : public FactoryBase {
public:
  Mat1Factory() = default;
  ~Mat1Factory() = default;
  boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
// 注册Mat1Factory
extern MAPPER::RegisterTool<Mat1Factory> _MAT1REGISTER("MAT1");
} // namespace FACTORY