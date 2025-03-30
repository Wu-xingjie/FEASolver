#pragma once
#include <assert.h>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include "component_factory/factory_base.h"
namespace MAPPER {
using func_compbase_ptr =
    std::function<boost::shared_ptr<FACTORY::FactoryBase>()>;
using comp_map = std::map<std::string, func_compbase_ptr>;

template <typename T> struct RegisterTool;
// 注册器调用接口
class Register {
public:
  static boost::shared_ptr<Register> inc();
  template <typename T> friend struct RegisterTool;
  comp_map GetMap() { return _file_to_comp; }

private:
  Register() = default;
  ~Register() = default;
  static comp_map _file_to_comp;
};
// 静态成员初始化
boost::shared_ptr<Register> Register::inc() {
  return boost::make_shared<Register>();
};
comp_map Register::_file_to_comp;

// 通过构造RegisterDatas对象实现注册器内添加数据
template <typename T> struct RegisterTool {
  RegisterTool(const std::string &name) {
    Register::inc()->GetMap().at(name) = []() { return boost::make_shared<T>(); };
  }
};
} // namespace MAPPER