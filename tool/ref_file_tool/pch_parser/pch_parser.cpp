#include "pch_parser.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <map>

void TOOL::PchParser::read_file() {
  std::fstream file;
  file.open(_pch_path, std::ios_base::in);
  if (file.is_open()) {
    while (file.good()) {
      // 获取pch信息
      std::string line;
      std::getline(file, line);
      // 解析pch每一行的信息
      int data_acount = 0;
      std::string data;
      pch_line line_info;
      line_info[0] = line.substr(0, 8);
      line_info[1] = line.substr(8, 8);
      line_info[2] = line.substr(16, 8);
      line_info[3] = line.substr(24, 8);
      line_info[4] = line.substr(32, 8);
      line_info[5] = line.substr(40);
      _pch_content.push_back(line_info);
    }
  } else {
    throw std::runtime_error(
        "[ERROR]:func(PchParser::read_file)>>>pch文件打开失败");
  }
}

Eigen::MatrixXd TOOL::PchParser::get_matrix() {
  Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(_matrix_dim, _matrix_dim);

  int col = 0;
  int row = 0;
  for (int i = 0; i < _pch_content.size(); i++) {
    pch_line line = _pch_content.at(i);

    auto idx = line.at(0).find("DMIG");
    if (idx != std::string::npos) { // 标识列号
      line.at(4).erase(std::remove(line.at(4).begin(), line.at(4).end(), ' '),
                       line.at(4).end());
      line.at(5).erase(std::remove(line.at(5).begin(), line.at(5).end(), ' '),
                       line.at(5).end());
      line.at(5).erase(std::remove(line.at(5).begin(), line.at(5).end(), '\r'),
                       line.at(5).end());
      int nid = boost::lexical_cast<int>(line.at(4));
      int dof = boost::lexical_cast<int>(line.at(5));
      col = 6 * (nid - 1) + dof;
    } else { // 标识行号以及矩阵元素值
      line.at(2).erase(std::remove(line.at(2).begin(), line.at(2).end(), ' '),
                       line.at(2).end());
      line.at(4).erase(std::remove(line.at(4).begin(), line.at(4).end(), ' '),
                       line.at(4).end());
      line.at(4).erase(std::remove(line.at(4).begin(), line.at(4).end(), '\r'),
                       line.at(4).end());
      int nid = boost::lexical_cast<int>(line.at(2));
      int dof = boost::lexical_cast<int>(line.at(4));
      row = 6 * (nid - 1) + dof;

      line.at(5).erase(std::remove(line.at(5).begin(), line.at(5).end(), ' '),
                       line.at(5).end());
      line.at(5).erase(std::remove(line.at(5).begin(), line.at(5).end(), '\r'),
                       line.at(5).end());

      matrix(row - 1, col - 1) = std::stod(line.at(5));
    }
  }

  return matrix;
}