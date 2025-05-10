#pragma once
#include "component/component_base.h"
#include "fea_model/fea_model.h"
namespace COMPONENT {
class LoadBase : public CompBase {
public:
  LoadBase() { _type = comp_type::load; }
  virtual ~LoadBase() = default;
  // 标记载荷类型
  enum class load_type { concentrated_force };
  load_type _load_type;
  virtual void SetComp(const file_data &datas) = 0;
  virtual void GenLoadVec(const MODEL::Model &model) = 0;
  virtual Eigen::VectorXd GetLoadVec() { return _load_vec; }
  virtual Eigen::VectorXd GetGLobalLoad(const MODEL::Model &model) = 0;
  virtual std::vector<int> GetNodes() = 0;

protected:
  Eigen::VectorXd _load_vec; // 载荷对应的载荷向量
};
} // namespace COMPONENT