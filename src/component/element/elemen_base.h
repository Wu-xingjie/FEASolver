#pragma once

namespace COMPONENT {
class ElemBase {
public:
  ElemBase() = default;
  ~ElemBase() = default;

protected:
  int _id;
  // TODO:等property完成后处理单元属性
};
} // namespace COMPONENT