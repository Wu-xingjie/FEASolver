#include "mapper.h"

#include "thread_pool/thread_pool.h"
#include <exception>
#include <iostream>

#include "regist_list.h"
namespace MAPPER {
void FileToMapper::mapper(MODEL::Model &model,
                          const std::vector<cards> &file_model) {
  // 创建线程池
  TOOL::ThreadPool thread_pool(4);
  for (auto &i : file_model) {
    try {
      // 创建任务
      auto mapper_task = [&](const cards &i) {
        auto comp_name = boost::any_cast<std::string>(i.front().at(0));
        auto comp_name_idx = _file_to_comp.find(comp_name);
        if (comp_name_idx == _file_to_comp.end()) {
          throw std::runtime_error(
              "[ERROR]:func(mapper)>>发现未处理的关键字: " + comp_name);
        }
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
      };
      // 往线程池中添加任务
      thread_pool.add_task(mapper_task, i);
    } catch (const std::exception &e) {
      std::cout << e.what() << '\n';
    }
  }
  // 关闭线程池
  std::cout << "准备关闭线程池"<<std::endl;
  thread_pool.close();
  std::cout << "[INFO]:(mapper)>>>线程池关闭" << std::endl;
}
} // namespace MAPPER