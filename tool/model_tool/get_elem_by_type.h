#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"
namespace TOOL {
std::vector<boost::shared_ptr<COMPONENT::ElemBase>> GetElemsByType(
    const MODEL::Model& model, const COMPONENT::ElemBase::elem_type& type) {
      std::vector<boost::shared_ptr<COMPONENT::ElemBase>> result;
  for (auto& comp : model._element){
    try
    {
     auto elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(comp);
     if (!elem){
      throw "从CompBase转型到ElemBase失败!";
     }
     if (elem->ElemType() == type){
      result.push_back(elem);
     }
    }
    catch(const char* e)
    {
      std::cout << "[ERROR]:单元" << comp->GetID() << e << '\n';
    }
  }
  return result;
}
}  // namespace TOOL