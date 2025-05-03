#include "fea_model.h"

#include "global_coordinate.h"
namespace MODEL {

Model::Model() {
  // 创建全局坐标系
  std::vector<std::array<boost::any, 10>> datas;
  std::array<boost::any, 10> arr_data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  datas.push_back(arr_data);
  auto gc = boost::make_shared<COMPONENT::GlobalCoord>();
  gc->SetComp(datas);
  _coord.push_back(gc);
}

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
  case COMPONENT::CompBase::comp_type::constrain:
    _constrain.push_back(p);
    break;

  default:
    break;
  }
}
} // namespace MODEL