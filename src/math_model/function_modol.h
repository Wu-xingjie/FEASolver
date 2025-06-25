#pragma once
#include <boost/shared_ptr.hpp>
#include <vector>

#include "exponential_func.h"
namespace MATH {
class FuncModel {
 public:
  FuncModel(const std::string& func);
  ~FuncModel() = default;

  void GenerateFuncModel();
  std::string CalculateFuc(const std::array<std::string, 3>& xyz);

  // 记录函数各项层次关系的函数树
  struct FuncTree {
    std::string _data;
    std::vector<boost::shared_ptr<FuncTree>> _sub_tree;
  };

 protected:
  //  递归构造函数树
  void Recrusion(const std::string& func);
  //  判断表达式是否需要分配。eg: (a+b)c -> ac+bc
  bool NeedAssign(const std::string& func);
  // 对需要调整的项进行函数分配调整，并返回调整后的各项。eg: (a+b)c -> ac+bc
  std::string AssignSubFunc(const std::string& func);
  // 判断表达式是否为最简表达式。 eg: 4*x^3
  bool IsSimplest(const std::string& func);

 private:
  boost::shared_ptr<FuncTree> _root;                     // 记录函数信息
  std::vector<boost::shared_ptr<MathBase>> _func_model;  // 记录函数模型信息
};
}  // namespace MATH