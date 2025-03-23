#include "parser.h"
#include "fstream"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <iostream>
namespace MAPPER {
void Parser::ParserFile(const std::string &file_address) {
  std::cout << "a" << std::endl;
  std::fstream file;
  file.open(file_address, std::ios_base::in);
  if (file.is_open()) {
    while (!file.good()) {
      std::string line;
      std::getline(file, line);
      std::vector<std::string> line_devided = DevideLine(line);
      std::vector<boost::any> temp_content;
      for (auto &elem : line_devided) {
        if (IsNumber(elem)) {
          temp_content.push_back(boost::lexical_cast<double>(elem));
        } else {
          temp_content.push_back(elem);
        }
      }
    }
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
  word.erase(std::remove(word.begin(), word.end(), '\0'), word.end());
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

std::vector<std::vector<boost::any>> &Parser::GetData() { return _file_data; }
} // namespace MAPPER