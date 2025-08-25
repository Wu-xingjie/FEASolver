#include <iostream>

#include "model_tool/display_matrixXd.h"
#include "ref_file_tool/pch_parser/get_refer_matrix.h"

int main(int argv, char* argc[]) {
  if (argv != 2) {
    std::cout << "[ERROR]>>输入参数必须为2!" << std::endl;
  } else {
    std::string pch_path = argc[1];
    int dim = 54;
    auto matrix = TOOL::GetMatrix(pch_path, dim);
    TOOL::DisplayMatrixXd(matrix, "test", true);
  }
  return 0;
}