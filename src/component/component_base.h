#pragma once
#include "fea_model/fea_model.h"
#include <array>
#include <boost/any.hpp>
#include <vector>

namespace COMPONENT {
typedef std::vector<std::array<boost::any, 10>> file_data;
// 所有元件都继承自该类
class CompBase {
public:
  CompBase() = default;
  virtual ~CompBase() = default;

  enum class comp_type { mat, prop, load, coord, element, node };

  virtual int GetID() = 0;
  virtual void SetComp(const file_data &datas) = 0;
  virtual comp_type Type() = 0;

  virtual void GenerateK(const MODEL::Model &model) = 0;

protected:
  // 记录元件类型
  comp_type _type;
};
} // namespace COMPONENT