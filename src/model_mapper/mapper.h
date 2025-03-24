#pragma once
#include "fea_model/fea_model.h"
#include "register.h"
namespace MAPPER {
class FileToMapper {
 public:
  FileToMapper() = default;
  ~FileToMapper() = default;

  void mapper(MODEL::Model& model,
              const std::vector<std::array<boost::any, 10>>& file_model);
};
}  // namespace MAPPER