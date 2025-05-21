#pragma once
#include <array>
#include <boost/optional.hpp>
#include <iostream>
#include <map>
#include <boost/any.hpp>
namespace TOOL {
// 根据E,NU,G三者之间的关系通过任意两个已知数推出另外一个未知数
std::array<double, 3> DealENuG(std::map<std::string, boost::any> datas);
} // namespace TOOL