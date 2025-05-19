#pragma once
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>

#include "component/component_base.h"
#include "fea_model/fea_model.h"
#include "model_mapper/register.h"

namespace COMPONENT {
class ElemBase : public CompBase {
 public:
  ElemBase() { _type = comp_type::element; };
  virtual ~ElemBase() = default;

  enum class elem_type { rod, bar };
  virtual void SetComp(const file_data &datas) = 0;
  virtual elem_type ElemType() = 0;
  virtual std::vector<int> GetNodes() = 0;
  virtual void GenerateK(const MODEL::Model &model) = 0;
  // 将局部坐标系下的单元刚度矩阵转换到全局坐标系下
  virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) = 0;

 protected:
  elem_type _elem_type;
};
}  // namespace COMPONENT