#pragma once
#include "src/component/element/elemen_base.h"
#include <array>
#include <map>
#include <string>

namespace TOOL {
std::map<std::array<int, 2>, std::array<std::string, 2>>
ElemIdx2Dof(COMPONENT::ElemBase &elem_base);
}