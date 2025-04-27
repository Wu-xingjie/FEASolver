#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"
namespace TOOL {
std::vector<boost::shared_ptr<COMPONENT::ElemBase>>
GetElemsByType(const MODEL::Model &model,
               const COMPONENT::ElemBase::elem_type &type);
} // namespace TOOL