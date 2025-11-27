#include "thread_pool.h"
#include <iostream>

namespace TOOL {
ThreadPool::ThreadPool(const int &thread_num) {
  _stop = false;
  for (int i = 0; i < thread_num; i++) {
    std::cout << "创建线程" << std::endl;
    _workers.push_back(std::thread(&ThreadPool::thread_work, this));
  }
}

ThreadPool::~ThreadPool() {
  // close();
  _stop = true;
  for (auto &td : _workers) {
    td.join();
  }
}

void ThreadPool::thread_work() {
  while (!_stop) {
    std::unique_lock<std::mutex> mtx(_mtx);
    _cond_val.wait(mtx,
                   [this]() { return (this->_stop) || !this->_tasks.empty(); });
    if (_stop && _tasks.empty()) {
      break;
    }
    auto task = _tasks.front();
    _tasks.pop();
    mtx.unlock();
    // 运行任务函数
    try {
      std::cout << "run task" << std::endl;
      task();
    } catch (const std::exception &e) {
      std::cout << e.what() << std::endl;
    }
    _cv_tasks_empty.notify_all();
  }
  std::cout << "[INFO]:func(ThreadPool::thread_work)>>>关闭线程："
            << std::this_thread::get_id() << std::endl;
}

void ThreadPool::close() {
  std::unique_lock<std::mutex> mtx(_mtx);
  _cv_tasks_empty.wait(mtx, [this]() { return this->_tasks.empty(); });
  _stop = true;
  _cond_val.notify_all();
}

} // namespace TOOL