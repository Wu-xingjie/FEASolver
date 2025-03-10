#pragma once

#include <iostream>
#include <set>
#include <stdexcept>

namespace TOOL {

// id类型
enum id_type {
  NODE,
  ELEMENT,
  PROPERTY,
  LOAD,
  MATERIAL,
  COORDINATE,
};

int IdManager(const id_type &type);

}  // namespace TOOL