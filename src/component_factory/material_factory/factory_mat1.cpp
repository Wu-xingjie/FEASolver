#include "factory_mat1.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> Mat1Factory::CreateComp() {
  return boost::make_shared<COMPONENT::MAT1>();
}
// 注册Mat1Factory
// MAPPER::RegisterTool<Mat1Factory> _MAT1REGISTER("MAT1");
} // namespace FACTORY