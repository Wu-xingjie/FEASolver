#include "mapper.h"

namespace MAPPER {
void FileToMapper::mapper(MODEL::Model& model,
                          const std::vector<cards>& file_model) {
  try {
    for (auto i : file_model) {
      auto comp_name = boost::any_cast<std::string>(i.front().at(0));
      // 根据注册表获取相应的元件工厂
      auto comp_fac = _file_to_comp.at(comp_name);
      // 创建并给元件赋值
      auto comp = comp_fac->CreateComp();
      comp->SetComp(i);
      // 将元件塞入到有限元模型中
      model.InsertComp(comp);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}
}  // namespace MAPPER