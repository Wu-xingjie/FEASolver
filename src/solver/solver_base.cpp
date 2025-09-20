#include "solver_base.h"

namespace SOLVER {
void SolverBase::GenCompLib(const std::string &file_path) {
  // 解析文件
  MAPPER::Parser parser;
  parser.ParserFile(file_path);
  parser.DividedByComp();
  auto comp_datas = parser.GetCompsData();
  // 文件映射到文件模型
  MAPPER::FileToMapper mapper;
  mapper.mapper(_model, comp_datas);
}
} // namespace SOLVER