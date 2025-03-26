#include "register.h"
#include <iostream>

int main() {
  MAPPER::File2ModelRegister register_test;

  auto it = register_test._file_to_model.find("MAT1");
  if (it != register_test._file_to_model.end()) {
    auto mat = it->second();
    
  }
  return 0;
}