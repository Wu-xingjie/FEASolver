#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace TOOL {
// 用于框架并行化的线程池
class ThreadPool {
public:
  ThreadPool(const int &thread_num);
  ~ThreadPool();

  void close();

  // 添加任务函数
  template <typename Func, typename... Args>
  void add_task(Func &&func, Args &&...args) {
    // 1: 打包任务函数
    // 利用std::bind将有参数有返回类型的可调用对象封装成无参数有返回类型的可调用对象
    // 利用通用引用(Func &&func, Args
    // &&...args)和std::forward实现完美转发，记录参数传入信息
    std::function<decltype(func(args...))()> first_package =
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    std::function<void()> task = [first_package]() { first_package(); };

    // 2: 将打包好的task塞入任务队列中
    std::unique_lock<std::mutex> mtx(_mtx);
    _tasks.emplace(task);
    _cond_val.notify_one();
  }

private:
  void thread_work();
  std::mutex _mtx;
  std::queue<std::function<void()>> _tasks;
  std::vector<std::thread> _workers;
  std::atomic<bool> _stop;
  std::condition_variable _cond_val;
  std::condition_variable _cv_tasks_empty;
};

} // namespace TOOL