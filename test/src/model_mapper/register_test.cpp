#include "register.h"

#include <iostream>

#include "component_factory/material_factory/factory_mat1.h"
// 注册MAT1
MAPPER::File2ModelRegister _MAT1REGIST("MAT1", [] {
  return boost::make_shared<FACTORY::Mat1Factory>();
});

int main() {
  // MAPPER::File2ModelRegister test_register;
  auto factory_mat1 = MAPPER::_file_to_model.at("MAT1")();
  auto mat1 = factory_mat1->CreateComp();
  std::cout << typeid(*mat1).name() << std::endl;

  return 0;
}