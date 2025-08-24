#include "model_tool/display_matrixXd.h"
#include "ref_file_tool/pch_parser/get_refer_matrix.h"

int main() {
  std::string pch_path =
      "/home/wxj/workspace/FEASolver/file/refer_case/cquad4_kf/cquad4_kf.pch";
  int dim = 54;
  auto matrix = TOOL::GetMatrix(pch_path, dim);
  TOOL::DisplayMatrixXd(matrix, "test", true);
  return 0;
}