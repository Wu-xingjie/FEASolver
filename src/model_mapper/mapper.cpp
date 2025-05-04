#include "mapper.h"
#include "regist_list.h"
#include <iostream>
namespace MAPPER {
void FileToMapper::mapper(MODEL::Model &model,
                          const std::vector<cards> &file_model) {
  for (auto &i : file_model) {
    try {
      auto comp_name = boost::any_cast<std::string>(i.front().at(0));
      // 根据注册表获取相应的元件工厂
      auto comp_fac = _file_to_comp.at(comp_name);
      // 创建并给元件赋值
      if (comp_fac) {
        auto comp = comp_fac->CreateComp();
        if (comp) {
          comp->SetComp(i);
          // 将元件塞入到有限元模型中
          model.InsertComp(comp);
        }
      }
    } catch (const std::exception &e) {
      std::cout << e.what() << '\n';
    }
  }
}
} // namespace MAPPER