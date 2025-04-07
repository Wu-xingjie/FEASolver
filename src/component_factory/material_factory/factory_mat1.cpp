#include "factory_mat1.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> Mat1Factory::CreateComp() {
  return boost::make_shared<COMPONENT::MAT1>();
}
}  // namespace FACTORY

// struct RegistMat1Factory {
//   RegistMat1Factory(const std::string& name) {
//     MAPPER::RegistComp<FACTORY::Mat1Factory>(name);
//   }
// };
// static RegistMat1Factory _RegistMat1Factory("MAT1");
