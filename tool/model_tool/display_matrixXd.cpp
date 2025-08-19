#include "display_matrixXd.h"
#include <iostream>
namespace TOOL {
void DisplayMatrixXd(const Eigen::MatrixXd &matrix, const std::string &title,
                     const bool &show_index) {
  std::cout << title << ":" << std::endl;
  if (!show_index) {
    for (int r = 0; r < matrix.rows(); r++) {
      for (int c = 0; c < matrix.cols(); c++) {
        std::cout << matrix(r, c) << ",";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  } else {
    for (int r = 0; r < matrix.rows(); r++) {
      for (int c = 0; c < matrix.cols(); c++) {
        std::cout << title << "<" << r << ", " << c << ">: " << matrix(r, c)
                  << std::endl;
      }
    }
  }
}
} // namespace TOOL