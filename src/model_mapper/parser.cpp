#include "parser.h"

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
      // for (auto &elem : line_devided) {
      //   if (IsNumber(elem)) {
      //     temp_content.push_back(boost::lexical_cast<double>(elem));
      //   } else {
      //     temp_content.push_back(elem);
      //   }
      // }
      for (int i = 0; i < line_devided.size(); i++) {
        if (IsNumber(line_devided.at(i))) {
          temp_content.at(i) = boost::lexical_cast<double>(line_devided.at(i));
        } else {
          temp_content.at(i) = line_devided.at(i);
        }
      }
      _file_data.push_back(temp_content);
    }
  } else {
    std::cout << "file open failed!" << std::endl;
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
}  // namespace MAPPER