#include "uniformly_distribute_surf_load.h"
#include "model_tool/get_comp_by_id.h"
namespace COMPONENT {
void UniDistributeSurfLoad::SetComp(const file_data &datas) {
  if (datas.size() > 1) {
    throw std::runtime_error("[ERROR]:func(UniDistributeSurfLoad::SetComp)>>>"
                             "均布面载荷暂不支持作用于节点数超过6的单元面");
  }
  auto card = datas.front();
  _id = boost::any_cast<int>(card.at(1));
  _load_x = boost::any_cast<int>(card.at(2));
  _load_y = boost::any_cast<int>(card.at(3));
  _load_z = boost::any_cast<int>(card.at(4));
  for (int i = 5; i < 11; i++) {
    int nid = boost::any_cast<int>(card.at(i));
    _surf_nids.push_back(nid);
  }
}

void UniDistributeSurfLoad::GenLoadVec(const MODEL::Model &model) {
  // FIXME:暂时只考虑施加在四边形面上的均布载荷
  if (_surf_nids.size() == 4) {
    // 创建四边形母单元的形函数矩阵

  } else {
    throw std::runtime_error("[ERROR]:func(UniDistributeSurfLoad::GenLoadVec)>>"
                             ">当前仅考虑施加在四边形面上的均布载荷");
  }
}

} // namespace COMPONENT