#include "thread_pool.h"
#include <iostream>

namespace TOOL {
ThreadPool::ThreadPool(const int &thread_num) {
  _stop = false;
  for (int i = 0; i < thread_num; i++) {
    _tasks.emplace(std::thread(thread_work));
  }
}

ThreadPool::~ThreadPool() {
  close();
  for (auto &td : _workers) {
    td.join();
  }
}

void ThreadPool::thread_work() {
  while (!_stop) {
    std::unique_lock<std::mutex> mtx(_mtx);
    _cond_val.wait(
        mtx, [this]() { return &(this->_stop) || !this->_tasks.empty(); });

    if (_stop && _tasks.empty()) {
      break;
    }
    auto task = _tasks.front();
    _tasks.pop();
    mtx.unlock();
    // 运行任务函数
    task();
  }
  std::cout << "[INFO]:func(ThreadPool::thread_work)>>>关闭线程："
            << std::this_thread::get_id() << std::endl;
}

void ThreadPool::close() {
  _stop = true;
  _cond_val.notify_all();
}

} // namespace TOOL