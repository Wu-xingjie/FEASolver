#pragma once
#include "src/component/element/elemen_base.h"
#include <map>
#include <string>

namespace TOOL {
std::map<int, std::string> ElemIdx2Dof(const COMPONENT::ElemBase &elem_base);
}