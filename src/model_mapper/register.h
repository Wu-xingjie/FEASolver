#pragma once
#include <assert.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <string>

#include "component_factory/factory_base.h"
// 文件到元件的映射表
static std::map<std::string,
                std::function<boost::shared_ptr<FACTORY::FactoryBase>()>>
    _file_to_model;
namespace MAPPER {

// 文件信息到有限元模型的注册表
// template <typename T>

class File2ModelRegister {
 public:
  File2ModelRegister() = default;
  // 定义有参构造函数，以此实现注册
  File2ModelRegister(
      std::string comp_name,
      std::function<boost::shared_ptr<FACTORY::FactoryBase>()> comp);
};
// template <typename T>
// std::map<std::string, std::function<boost::shared_ptr<FACTORY::FactoryBase>()>>
//     File2ModelRegister::_file_to_model;
}  // namespace MAPPER