#include "component/coordinate/cartesian_coordinate.h"
#include "component/element/D1/ROD.h"
#include "geometry_tool/length_node_to_node.h"
#include "component/material/mat1.h"
#include "component/node/node.h"
#include "component/property/PROD.h"
#include <iostream>
int main() {
//   // 创建材料
//   COMPONENT::MAT1 mat1;
//   mat1.SetMaterial(10000, 211, 0.3);

//   // 创建属性
//   COMPONENT::PROD prod;
//   prod.SetProperty(2.1, mat1);

//   // 创建节点
//   // 创建坐标系1
//   COMPONENT::vec_3 test_origin(1, 2, 4);
//   COMPONENT::vec_3 test_vec1(1, 0, 0);
//   COMPONENT::vec_3 test_vec2(0, 0, -1);
//   COMPONENT::vec_3 test_vec3(0, 1, 0);
//   COMPONENT::CartesianCoord coord1(test_origin, test_vec1, test_vec2,
//                                    test_vec3);
//   // 创建节点
//   COMPONENT::Node n1(1, 2, 3, coord1);
//   COMPONENT::Node n2(2, 5, 6, coord1);
//   std::cout << "n1到n2距离: " << TOOL::LenNode2Node(n1, n2) << std::endl;

//   // 创建单元
//   COMPONENT::ROD rod;
//   rod.SetNode(n1, n2);
//   rod.SetProperty(prod);
//   rod.GenerateK();
//   std::cout << "单元刚度矩阵： " << std::endl << rod.GetK() << std::endl;

  return 0;
}