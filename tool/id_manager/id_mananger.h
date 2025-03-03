#pragma once

#include <iostream>
#include <set>
#include <stdexcept>

namespace TOOL {

// id类型
enum id_type {
  NODE,
  ELEMENT,
  LOAD,
  COORDINATE,
};

int IdManager(const id_type &type);

}  // namespace TOOL