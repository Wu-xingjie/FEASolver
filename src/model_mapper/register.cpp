#include "register.h"

namespace MAPPER {
  File2ModelRegister::File2ModelRegister(
    std::string comp_name,
    std::function<boost::shared_ptr<FACTORY::FactoryBase>()> comp) {
  _file_to_model[comp_name] = comp;
}
}  // namespace MAPPER