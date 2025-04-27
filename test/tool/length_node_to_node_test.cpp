#include "component/coordinate/cartesian_by_node.h"
#include "geometry_tool/length_node_to_node.h"

int main() {
  // std::cout << "***********同一坐标系下**********" << std::endl;
  // // 创建坐标系1
  // COMPONENT::vec_3 test_origin(1, 2, 4);
  // COMPONENT::vec_3 test_vec1(1, 0, 0);
  // COMPONENT::vec_3 test_vec2(0, 0, -1);
  // COMPONENT::vec_3 test_vec3(0, 1, 0);
  // COMPONENT::CartesianCoord coord1(test_origin, test_vec1, test_vec2,
  //                                  test_vec3);
  // // 创建节点
  // COMPONENT::Node n1(1, 2, 3, coord1);
  // COMPONENT::Node n2(2, 5, 6, coord1);

  // std::cout << "同一坐标系下的两节点距离：" << TOOL::LenNode2Node(n1, n2)
  //           << std::endl;

  // std::cout << "***********不同坐标系下**********" << std::endl;
  // // 创建坐标系2
  // COMPONENT::vec_3 test2_origin(3, 5, 6);
  // COMPONENT::vec_3 test2_vec1(0, 0, 1);
  // COMPONENT::vec_3 test2_vec2(0, 1, 0);
  // COMPONENT::vec_3 test2_vec3(-1, 0, 0);
  // COMPONENT::CartesianCoord coord2(test2_origin, test2_vec1, test2_vec2,
  //                                  test2_vec3);
  // COMPONENT::Node n3(2, 5, 6, coord2);
  // std::cout << "不同坐标系下的两节点距离：" << TOOL::LenNode2Node(n3, n2)
  //           << std::endl;
  return 0;
}