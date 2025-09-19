#pragma once
#include <array>
#include <boost/any.hpp>
#include <string>
#include <vector>
namespace MAPPER {
using cards = std::vector<std::array<boost::any, 10>>;
// 文件解析器，负责解析文件信息
class Parser {
 public:
  Parser() = default;
  ~Parser() = default;
  // 解析文件
  void ParserFile(const std::string &file_address);
  // 获取文件解析结果
  cards &GetData();

  // 将文件数据根据元件进行划分
  void DividedByComp();

  // 获得根据元件划分的文件信息
  std::vector<cards> GetCompsData() { return _comp_data; }

 protected:
  // 判断字符串是否为一个数
  bool IsNumber(const std::string &l);
  // 判断字符串是否为一个整数
  bool IsInt(const std::string& l);
  // 去除字符串中的空格
  void RemoveBlack(std::string &word);
  // 将字符串解析为多个子字符串
  std::vector<std::string> DevideLine(const std::string &line);
  // 判断一个字符串是否都是空格,用于处理输入文件中数据项为空的情况
  bool IsEmpty(const std::string &data);

 private:
  cards _file_data;
  std::vector<cards> _comp_data;
};
}  // namespace MAPPER