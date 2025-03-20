#pragma once
#include <assert.h>

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <string>

#include "component/component_base.h"
namespace MAPPER {

class FileParser;

// 文件信息到有限元模型的注册表
template <typename T>
class File2ModelRegister {
 public:
  File2ModelRegister() = default;
  ~File2ModelRegister() = default;
  // 定义有参构造函数，以此实现注册
  File2ModelRegister(std::string comp_name,
                     boost::shared_ptr<COMPONENT::CompBase> comp) {
    try {
      if (comp) {
        throw comp_name + "注册时发生错误 !";
      }
      static_assert(std::is_base_of<COMPONENT::CompBase, T>::value,
                    "CompBase is not base of " + comp_name);
      _file_to_model.insert(
          std::make_pair<std::string, boost::shared_ptr<COMPONENT::CompBase>>(
              comp_name, comp));
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
  }

  // 将解析器设置为友元从而允许解析器访问映射表

  friend class FileParser;

 private:
  // 文件到元件的映射表
  static std::map<std::string, boost::shared_ptr<COMPONENT::CompBase>>
      _file_to_model;
};
template <typename T>
std::map<std::string, boost::shared_ptr<COMPONENT::CompBase>>
    File2ModelRegister<T>::_file_to_model;
}  // namespace MAPPER