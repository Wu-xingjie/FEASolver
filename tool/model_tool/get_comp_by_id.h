#include "component/element/elemen_base.h"
#include "component/material/material_base.h"
#include "component/node/node.h"
#include "component/property/property_base.h"
#include "fea_model/fea_model.h"
namespace TOOL {
boost::shared_ptr<COMPONENT::CompBase>
GetCompById(const MODEL::Model &model,
            const COMPONENT::CompBase::comp_type &type, const int &id) {
  boost::shared_ptr<COMPONENT::CompBase> result;
  // 获取特定类型
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> comps;

  auto FindInType =
      [&result](const int &i,
                std::vector<boost::shared_ptr<COMPONENT::CompBase>> cps) {
        for (auto cp : cps) {
          if (cp->GetID() == i) {
            result = cp;
            break;
          }
        }
      };

  if (type == COMPONENT::CompBase::comp_type::mat) {
    comps = model._mat;
    // FindInType(id, comps);
    for (auto i : comps) {
      if (i->GetID() == id) {
        result = i;
        break;
      }
    }

  } else if (type == COMPONENT::CompBase::comp_type::prop) {
    comps = model._prop;
    // FindInType(id, comps);
    for (auto i : comps) {
      if (i->GetID() == id) {
        result = i;
        break;
      }
    }

  } else if (type == COMPONENT::CompBase::comp_type::load) {
    comps = model._load;
    // FindInType(id, comps);
    for (auto i : comps) {
      if (i->GetID() == id) {
        result = i;
        break;
      }
    }

  } else if (type == COMPONENT::CompBase::comp_type::coord) {
    comps = model._coord;
    // FindInType(id, comps);
    for (auto i : comps) {
      if (i->GetID() == id) {
        result = i;
        break;
      }
    }

  } else if (type == COMPONENT::CompBase::comp_type::element) {
    comps = model._element;
    // FindInType(id, comps);
    for (auto i : comps) {
      if (i->GetID() == id) {
        result = i;
        break;
      }
    }

  } else if (type == COMPONENT::CompBase::comp_type::node) {
    comps = model._node;
    // FindInType(id, comps);
    for (auto i : comps) {
      if (i->GetID() == id) {
        result = i;
        break;
      }
    }
  }

  return result;
}
} // namespace TOOL