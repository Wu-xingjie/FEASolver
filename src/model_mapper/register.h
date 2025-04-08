#pragma once
#include <assert.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include "component_factory/factory_base.h"
#include "component_factory/material_factory/factory_mat1.h"

namespace MAPPER {
using fac = boost::shared_ptr<FACTORY::FactoryBase>;
using comp_map = std::map<std::string, fac>;

static comp_map _file_to_comp;
template<typename T>
struct Regist{
  Regist(const std::string& name){
    _file_to_comp[name] = boost::make_shared<T>();
  }
};
}  // namespace MAPPER