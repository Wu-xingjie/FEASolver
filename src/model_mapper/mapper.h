#pragma once
#include "fea_model/fea_model.h"
#include "register.h"
#include "parser.h"
namespace MAPPER {
class FileToMapper {
 public:
  FileToMapper() = default;
  ~FileToMapper() = default;

  void mapper(MODEL::Model& model,
              const std::vector<cards>& file_model);
};
}  // namespace MAPPER