#pragma once
#include "component/element/elemen_base.h"
#include "component/material/material_base.h"
#include "component/node/node.h"
#include "component/property/property_base.h"
#include "fea_model/fea_model.h"
namespace TOOL {
boost::shared_ptr<COMPONENT::CompBase>
GetCompById(const MODEL::Model &model,
            const COMPONENT::CompBase::comp_type &type, const int &id);
} // namespace TOOL