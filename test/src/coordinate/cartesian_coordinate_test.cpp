#include "coordinate/cartesian_coordinate.h"

#include <iostream>
int main() {
  typedef COORDINATE::arr3 test_arr3;
  test_arr3 test_origin{1.1, 2.3, 43.2};
  test_arr3 test_vec1{1.32, 2.2, 4.1};
  test_arr3 test_vec2{1.14, 6.4, 3.2};
  test_arr3 test_vec3{1.15, 4.9, 4.2};

  COORDINATE::CartesianCoord test(test_origin, test_vec1, test_vec2, test_vec3);

  test_arr3 result_origin = test.getORIGIN();
  std::array<test_arr3, 3> test_vec = test.getVEC();

  std::cout << "*************构造函数构造*************" << std::endl;
  // 测试坐标系id
  std::cout << "coord_id: " << test.getID() << std::endl;
  // 测试坐标原点
  std::cout << "coord_origin: ";
  for (auto elem : result_origin) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
  // 测试坐标轴
  std::cout << "coord_axis: " << std::endl;
  for (int i = 1; i < 4; i++) {
    std::cout << "axis" << i << ": ";
    for (auto elem : test_vec.at(i - 1)) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "*************接口测试*************" << std::endl;
  COORDINATE::CartesianCoord test1;

  test_arr3 test1_origin{1.1, 2.3, 43.2};
  test_arr3 test1_vec1{1.32, 2.2, 4.1};
  test_arr3 test1_vec2{1.14, 6.4, 3.2};
  test_arr3 test1_vec3{1.15, 4.9, 4.2};
  test1.setORIGIN(test1_origin);
  test1.setVEC(test1_vec1, test1_vec2, test1_vec3);

  test_arr3 result1_origin = test.getORIGIN();
  std::array<test_arr3, 3> test1_vec = test.getVEC();

  // 测试坐标系id
  std::cout << "coord_id: " << test1.getID() << std::endl;
  // 测试坐标原点
  std::cout << "coord_origin: ";
  for (auto elem : result1_origin) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
  // 测试坐标轴
  std::cout << "coord_axis: " << std::endl;
  for (int i = 1; i < 4; i++) {
    std::cout << "axis" << i << ": ";
    for (auto elem : test1_vec.at(i - 1)) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "赋值运算符测试：" << std::endl;
  COORDINATE::CartesianCoord temp;
  temp = test1;
  test_arr3 temp_origin = temp.getORIGIN();
  std::array<test_arr3, 3> temp_vec = temp.getVEC();


  // 测试坐标系id
  std::cout << "coord_id: " << temp.getID() << std::endl;
  // 测试坐标原点
  std::cout << "coord_origin: ";
  for (auto elem : result1_origin) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
  // 测试坐标轴
  std::cout << "coord_axis: " << std::endl;
  for (int i = 1; i < 4; i++) {
    std::cout << "axis" << i << ": ";
    for (auto elem : temp_vec.at(i - 1)) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}