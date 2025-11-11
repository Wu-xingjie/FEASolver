#include "parser.h"
#include <future>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>

namespace MAPPER {
void Parser::ParserFile(const std::string &file_address) {
  std::fstream file;
  file.open(file_address, std::ios_base::in);

  if (file.is_open()) {
    while (file.good()) {
      std::string line;
      std::getline(file, line);
      std::vector<std::string> line_devided = DevideLine(line);
      std::array<boost::any, 10> temp_content;
      for (int i = 0; i < line_devided.size(); i++) {
        if (IsNumber(line_devided.at(i))) {
          if (IsInt(line_devided.at(i))) {
            temp_content.at(i) = boost::lexical_cast<int>(line_devided.at(i));
          } else {
            temp_content.at(i) =
                boost::lexical_cast<double>(line_devided.at(i));
          }
        } else {
          temp_content.at(i) = line_devided.at(i);
        }
      }
      _file_data.push_back(temp_content);
    }
  } else {
    throw std::runtime_error(
        "[ERROR]:func(Parser::ParserFile)>>>计算文件打开失败");
  }
}

bool Parser::IsNumber(const std::string &l) {
  bool is_number = true;
  for (auto &i : l) {
    if (isalpha(i)) {
      is_number = false;
      break;
    }
  }
  return is_number;
}

bool Parser::IsInt(const std::string &l) {
  bool result = true;
  if (IsNumber(l)) {
    for (auto &i : l) {
      if (i == '.') {
        result = false;
      }
    }
  } else {
    result = false;
  }
  return result;
}

void Parser::RemoveBlack(std::string &word) {
  word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
}

std::vector<std::string> Parser::DevideLine(const std::string &line) {
  std::vector<std::string> result;
  std::string word;
  for (auto c : line) {
    if (c != ',') {
      word.push_back(c);
    } else {
      if (IsEmpty(word)) {
        word = "null";
      }
      RemoveBlack(word);
      result.push_back(word);
      word.clear();
    }
  }
  RemoveBlack(word);
  result.push_back(word);
  return result;
}

std::vector<std::array<boost::any, 10>> &Parser::GetData() {
  return _file_data;
}

bool Parser::IsEmpty(const std::string &data) {
  bool result = true;
  for (auto &c : data) {
    if (c != ' ') {
      result = false;
      break;
    }
  }
  return result;
}

void Parser::DividedByComp() {
  cards comp;
  for (int i = 0; i < _file_data.size() - 1; i++) {
    auto curline = _file_data.at(i);
    auto nexline = _file_data.at(i + 1);
    comp.push_back(curline);
    if (boost::any_cast<std::string>(nexline.at(0)) != "continue") {
      _comp_data.push_back(comp);
      comp.clear();
    }
  }
  // 处理最后一行
  auto lasline = _file_data.back();
  if (boost::any_cast<std::string>(lasline.at(0)) != "continue") {
    comp.push_back(lasline);
    _comp_data.push_back(comp);
    comp.clear();
  } else {
    comp.push_back(lasline);
    _comp_data.push_back(comp);
  }
}

} // namespace MAPPER