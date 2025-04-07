#include "register.h"

#include <iostream>

#include "component_factory/material_factory/factory_mat1.h"
#include "regist_list.h"

int main() {
  if (!MAPPER::_file_to_comp.empty()) {
    auto fac = MAPPER::_file_to_comp.at("MAT1");
    auto mat1 = fac->CreateComp();
    std::cout << "id: " << mat1->GetID() << std::endl;
    std::cout << ">>>>>太好了，终于注册好了！！！" << std::endl;
  } else {
    std::cout << ">>>>>狗东西！还是没注册好！！！" << std::endl;
  }
  return 0;
}