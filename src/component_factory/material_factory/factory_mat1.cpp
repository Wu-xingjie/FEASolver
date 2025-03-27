#include "factory_mat1.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> Mat1Factory::CreateComp() {
  auto comp = boost::make_shared<COMPONENT::MAT1>();
  return comp;
}
// 注册MAT1
// MAPPER::File2ModelRegister _MAT1REGIST("MAT1", [] {
//   return boost::make_shared<Mat1Factory>();
// });
}  // namespace FACTORY