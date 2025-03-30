#include "component_factory/material_factory/factory_mat1.h"
#include "register.h"
#include <iostream>

int main() {
  MAPPER::Register test_register;
  auto fac = test_register._file_to_comp.at("MAT1")();
  auto mat1 = fac->CreateComp();
  std::cout << "id: " << mat1->GetID() << std::endl;
  return 0;
}