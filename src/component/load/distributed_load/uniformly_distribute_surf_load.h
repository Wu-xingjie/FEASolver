#pragma once
#include "component/load/load_base.h"
#include <eigen3/Eigen/Dense>
#include <vector>

namespace COMPONENT {
// 单元均布面载荷
class UniDistributeSurfLoad : public LoadBase {
public:
  UniDistributeSurfLoad() {
    _load_type = LoadBase::load_type::uniform_distribute__surf_load;
  }
  virtual ~UniDistributeSurfLoad() = default;
  virtual void SetComp(const file_data &datas) override;
  virtual void GenLoadVec(const MODEL::Model &model) override;
  virtual Eigen::VectorXd GetGLobalLoad(const MODEL::Model &model) override;
  virtual std::vector<int> GetNodes();

private:
  double _load_x;
  double _load_y;
  double _load_z;
  // 载荷作用面的节点，节点输入顺序需要规范化：
  // 三相限：1, 四相限：2, 一相限：3, 二相限：4,
  std::vector<int> _surf_nids;
};
} // namespace COMPONENT