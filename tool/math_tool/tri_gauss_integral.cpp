#include "tri_gauss_integral.h"

#include <func_expr/func_cal.h>

#include <boost/make_shared.hpp>
#include <map>
#include <stdexcept>

#include "geometry_tool/area_of_three_points.h"

namespace TOOL {
// 厚度方向高斯点个数和高斯点之间的映射关系
std::map<int, std::vector<double>> t_samp_point_map = {
    {1, {0.0}}, {3, {-0.7746, 0.0, 0.7746}}};
// 厚度方向高斯点个数和权值之间的映射关系
std::map<int, std::vector<double>> t_weight_map = {
    {1, {2.0}}, {3, {0.555555, 0.888888, 0.555555}}};
// 平面内高斯点个数和高斯点之间的映射关系
std::map<int, std::vector<std::vector<double>>> plane_samp_point_map = {
    {1, {{0.333333, 0.333333}}}, {3, {{0.5, 0.0}, {0.0, 0.5}, {0.5, 0.5}}}};
// 平面内高斯点个数和权值之间的映射关系
std::map<int, std::vector<double>> plane_weight_map = {
    {1, {0.5}}, {3, {0.166667, 0.166667, 0.166667}}};

boost::shared_ptr<double> TriGaussIntegral(
    const std::string& func, const std::vector<Eigen::Vector2d>& points,
    const int& num_intergral_point, const double& t) {
  auto result = boost::make_shared<double>();
  // 根据积分点个数确定参考域积分点
  if (num_intergral_point != 1 || num_intergral_point != 3) {
    throw std::runtime_error(
        "[ERROR]:func(TOOL::TriGaussIntegral)>>>"
        "暂只支持平面内一个或三个积分点的高斯积分!");
  }
  auto plane_sample = plane_samp_point_map.at(num_intergral_point);
  auto plane_weight = plane_weight_map.at(num_intergral_point);
  auto t_sample = t_samp_point_map.at(num_intergral_point);
  auto t_weight = t_weight_map.at(num_intergral_point);

  // 计算三点构成的物理域面积
  if (points.size() != 3) {
    throw std::runtime_error(
        "[ERROR]:func(TOOL::TriGaussIntegral)>>>"
        "物理域顶点信息不充足!");
  }
  auto point_1 = points.at(0);
  auto point_2 = points.at(1);
  auto point_3 = points.at(2);
  auto area = AreaOfThreePoints(point_1, point_2, point_3);
  if (!area) {
    throw std::runtime_error(
        "[ERROR]:func(TOOL::TriGaussIntegral)>>>"
        "计算物理域面积失败!");
  }

  // 生成P矩阵(P矩阵参考理论推导草稿)
  Eigen::Matrix2d matrix_p;
  matrix_p(0, 0) = point_2(1) - point_1(1);
  matrix_p(0, 1) = point_1(0) - point_2(0);
  matrix_p(1, 0) = point_1(1) - point_3(1);
  matrix_p(1, 1) = point_3(0) - point_1(0);
  // 生成Q向量(Q向量参考理论推导草稿)
  Eigen::Vector2d vector_q;
  vector_q(0) = point_1(0) * point_2(1) - point_1(1) * point_2(0);
  vector_q(1) = point_3(0) * point_1(1) - point_3(1) * point_1(0);
  // 生成Jacob矩阵
  auto matrix_p_inv = matrix_p.inverse();
  Eigen::Matrix3d matrix_jacob = Eigen::Matrix3d::Zero(3, 3);
  matrix_jacob(0, 0) = matrix_p_inv(0, 0) * (*area);
  matrix_jacob(0, 1) = matrix_p_inv(0, 1) * (*area);
  matrix_jacob(1, 0) = matrix_p_inv(1, 0) * (*area);
  matrix_jacob(1, 1) = matrix_p_inv(1, 1) * (*area);
  matrix_jacob(2, 2) = t * 0.5;
  auto det_jacob = matrix_jacob.determinant();

  // 物理域下中面的积分点
  std::vector<Eigen::Vector2d> phy_plane_samples;
  for (auto elem : plane_sample) {
    Eigen::Vector2d temp{elem.at(0), elem.at(1)};
    phy_plane_samples.push_back(temp);
  }
  // 物理域下厚度方向的积分点
  std::vector<double> phy_t_samples;
  for (auto elem : t_sample) {
    phy_t_samples.push_back(0.5 * t * elem);
  }

  // 计算积分点处函数值
  std::vector<double> func_samp;
  for (int i = 0; i < num_intergral_point; i++) {
    func_samp.push_back(FuncCal(func, phy_plane_samples.at(i)(0),
                                phy_plane_samples.at(i)(1),
                                phy_t_samples.at(i)));
  }
  // 计算高斯积分
  for (int i = 0; i < num_intergral_point; i++) {
    *result +=
        plane_weight.at(i) * t_weight.at(i) * func_samp.at(i) * det_jacob;
  }
  return result;
}
}  // namespace TOOL