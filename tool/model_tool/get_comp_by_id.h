#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"
namespace TOOL {
boost::shared_ptr<COMPONENT::CompBase> GetCompById(
    const MODEL::Model& model, const COMPONENT::CompBase::comp_type& type,
    const int& id) {
  boost::shared_ptr<COMPONENT::CompBase> result;
  // 获取特定类型
  std::vector<boost::shared_ptr<COMPONENT::CompBase>> comps;
  switch (type) {
    case COMPONENT::CompBase::comp_type::mat:
      comps = model._mat;
      break;
    case COMPONENT::CompBase::comp_type::prop:
      comps = model._prop;
      break;
    case COMPONENT::CompBase::comp_type::load:
      comps = model._load;
      break;
    case COMPONENT::CompBase::comp_type::coord:
      comps = model._coord;
      break;
    case COMPONENT::CompBase::comp_type::element:
      comps = model._element;
      break;
    case COMPONENT::CompBase::comp_type::node:
      comps = model._node;
      break;
    default:
      break;
  }
  // 特定类型中搜索指定的id
  for (auto& comp : comps) {
    try {
      auto elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(comp);
      if (!elem) {
        throw "从CompBase转型到ElemBase失败!";
      }
      if (elem->GetID() == id) {
        result = elem;
      }
    } catch (const char* e) {
      std::cout << "[ERROR]:单元" << comp->GetID() << e << '\n';
    }
  }
  return result;
}
}  // namespace TOOL