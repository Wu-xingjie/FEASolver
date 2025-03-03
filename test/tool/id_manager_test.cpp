#include <iostream>

#include "id_manager/id_mananger.h"

int main() {
  std::cout << ">>>>>>>NODE test" << std::endl;
  std::cout << "NODE: " << TOOL::IdManager(TOOL::id_type::NODE) << std::endl;
  std::cout << "NODE: " << TOOL::IdManager(TOOL::id_type::NODE) << std::endl;
  std::cout << "NODE: " << TOOL::IdManager(TOOL::id_type::NODE) << std::endl;

  std::cout << ">>>>>>>ELEMENT test" << std::endl;
  std::cout << "ELMENT: " <<TOOL::IdManager(TOOL::id_type::ELEMENT) << std::endl;
  std::cout << "ELMENT: " <<TOOL::IdManager(TOOL::id_type::ELEMENT) << std::endl;
  std::cout << "ELMENT: " <<TOOL::IdManager(TOOL::id_type::ELEMENT) << std::endl;

  std::cout << ">>>>>>>LOAD test" << std::endl;
  std::cout << "LOAD: " << TOOL::IdManager(TOOL::id_type::LOAD) << std::endl;
  std::cout << "LOAD: " << TOOL::IdManager(TOOL::id_type::LOAD) << std::endl;
  std::cout << "LOAD: " << TOOL::IdManager(TOOL::id_type::LOAD) << std::endl;

  std::cout << ">>>>>>>COORDINATE test" << std::endl;
  std::cout << "COORDINATE: " << TOOL::IdManager(TOOL::id_type::COORDINATE) << std::endl;
  std::cout << "COORDINATE: " << TOOL::IdManager(TOOL::id_type::COORDINATE) << std::endl;
  std::cout << "COORDINATE: " << TOOL::IdManager(TOOL::id_type::COORDINATE) << std::endl;
  return 0;
}
