#include "tool/id_manager/id_mananger.h"
#include <iostream>

namespace TEST {
int main() {
  std::cout << TOOL::IdManager(TOOL::id_type::NODE) << std::endl;
  return 0;
}
} // namespace TEST