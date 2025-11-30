#include "thread_pool.h"
#include <iostream>

namespace TOOL {
ThreadPool::ThreadPool(const int thread_num, const std::string &thread_user) {
  _stop = false;
  for (int i = 0; i < thread_num; i++) {
    if (thread_user != "null") {
      std::cout << "[INFO]:func(ThreadPool::ThreadPool)>>>" << thread_user
                << "创建线程" << std::endl;
    } else {
      std::cout << "[INFO]:func(ThreadPool::ThreadPool)>>>" << "创建线程"
                << std::endl;
    }
    _workers.emplace_back(std::thread(&ThreadPool::thread_work, this));
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
    if (_tasks.empty()) {
      _cond_val.wait(mtx);
    }
    if (_stop && _tasks.empty()) {
      break;
    }
    auto task = _tasks.front();
    _tasks.pop();
    mtx.unlock();
    // 运行任务函数
    try {
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