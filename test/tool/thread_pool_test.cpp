#include "thread_pool/thread_pool.h"
#include <chrono>
#include <iostream>
void test_func1() {
  std::this_thread::sleep_for(std::chrono::seconds(8));
  std::cout << "thread_id:" << std::this_thread::get_id() << std::endl;
  std::cout << "成功运行函数" << std::endl;
}

int main() {
  // 创建线程池
  TOOL::ThreadPool test_thread_pool(4);
  for (int i = 0; i < 15; i++) {
    test_thread_pool.add_task(test_func1);
  }
  test_thread_pool.close();
  std::cout << "[INFO]>>>运行成功！" << std::endl;
  return 0;
}