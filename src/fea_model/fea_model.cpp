#include "fea_model.h"

namespace MODEL {
void Model::InsertComp(const boost::shared_ptr<COMPONENT::CompBase> p) {
  auto comp_type = p->Type();
  switch (comp_type) {
    case COMPONENT::CompBase::comp_type::mat:
      _mat.push_back(p);
      break;
    case COMPONENT::CompBase::comp_type::prop:
      _prop.push_back(p);
      break;
    case COMPONENT::CompBase::comp_type::load:
      _load.push_back(p);
      break;
    case COMPONENT::CompBase::comp_type::coord:
      _coord.push_back(p);
      break;
    case COMPONENT::CompBase::comp_type::element:
      _element.push_back(p);
      break;
    case COMPONENT::CompBase::comp_type::node:
      _node.push_back(p);
      break;

    default:
      break;
  }
}
}  // namespace MODEL