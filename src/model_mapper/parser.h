#pragma once
#include <boost/any.hpp>
#include <string>
#include <vector>
namespace MAPPER {
// 文件解析器，负责解析文件信息
class Parser {
public:
  Parser() = default;
  ~Parser() = default;
  // 解析文件
  void ParserFile(const std::string &file_address);

protected:
  bool IsNumber(const std::string &l);
  void RemoveBlack(std::string &word);
  // 将字符串解析为多个子字符串
  std::vector<std::string> DevideLine(const std::string &line);

private:
  std::vector<std::vector<boost::any>> _file_data;
};
} // namespace MAPPER