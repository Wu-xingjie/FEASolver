#include "component_factory/material_factory/factory_mat1.h"
#include "register.h"
#include <iostream>

static const MAPPER::RegisterTool<FACTORY::Mat1Factory> _MAT1REGISTER("MAT1");

static std::map<std::string, boost::shared_ptr<std::string>> a{
    {"sdfa", boost::make_shared<std::string>("slakdjf")}};

int main() {
  MAPPER::Register test_register;
  auto factory = test_register._file_to_comp.at("MAT1");
  std::cout << a.at("sdfa") << std::endl;

  if (factory) {
    auto mat1 = factory->CreateComp();
    // auto id = mat1->GetID();
    std::cout << typeid(*mat1).name() << std::endl;
  }
  return 0;
}