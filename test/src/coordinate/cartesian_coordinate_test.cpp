#include "/home/wxj/workspace/FEASolver/src/coordinate/cartesian_coordinate.h"

int main() {
  typedef COORDINATE::arr3 test_arr3;
  test_arr3 test_origin{1.1, 2.3, 43.2};
  test_arr3 test_vec1{1.32, 2.2, 4.1};
  test_arr3 test_vec2{1.14, 6.4, 3.2};
  test_arr3 test_vec3{1.15, 4.9, 4.2};

  COORDINATE::CartesianCoord test(test_origin, test_vec1, test_vec2, test_vec3);

  test_arr3 result_origin = test.getORIGIN();
  std::array<test_arr3, 3> test_vec = test.getVEC();
  
  //测试result_origin，test_vec
  

  return 0;
}