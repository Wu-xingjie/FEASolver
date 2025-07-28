#include "cos_of_vectors.h"
#include <eigen3/Eigen/Dense>

double TOOL::CosOfVecs(const Eigen::Vector3d &v1, const Eigen::Vector3d &v2) {
  try {
    if (v1.size() != 3 || v2.size() != 3) {
      throw "vector's size is not 3, please check!";
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  double a = v1.dot(v2);
  return (v1.dot(v2) / (v1.norm() * v2.norm()));
}