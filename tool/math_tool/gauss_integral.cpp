#include "gauss_integral.h"

#include <array>
#include <boost/make_shared.hpp>
#include <map>
#include <regex>
#include <stdexcept>

#include "func_expr/func_cal.h"
namespace TOOL {
// 高斯点个数和高斯点之间的映射关系
std::map<int, std::vector<double>> samp_point_map = {
    {1, {0.0}},
    {2, {-0.57735, 0.57735}},
    {3, {-0.7746, 0.0, 0.7746}},
    {4, {-0.861136, -0.339981, 0.339981, 0.861136}}};
// 高斯点个数和权值之间的映射关系
std::map<int, std::vector<double>> weight_map = {
    {1, {2.0}},
    {2, {1.0, 1.0}},
    {3, {0.555555, 0.888888, 0.555555}},
    {4, {0.347855, 0.652145, 0.652145, 0.347855}}};

boost::shared_ptr<double>
GaussIntegral(const std::string &func, const int &num_r, const int &num_integal) {
  auto result = boost::make_shared<double>();
  if (num_r > 4) {
    throw std::runtime_error(
        "[ERROR]:func(TOOL::GaussIntegral)>>>暂只支持1-4个采样点的高斯积分!");
  }
  if (num_integal > 4) {
    throw std::runtime_error(
        "[ERROR]:func(TOOL::GaussIntegral)>>>积分最高为三重积分!");
  }
  // 获取范围[-1, 1]下的高斯点和权值
  auto samples = samp_point_map.at(num_r);
  auto weights = weight_map.at(num_r);

  // 构建三维空间的高斯采样点集合
  std::vector<std::vector<double>> Dim3_samples = {samples, samples, samples};

  // 构建三维空间的高斯采样点集合
  std::vector<std::vector<double>> Dim3_weight;
  // 三维高斯采样点集合初始化
  for (int i = 0; i < 3; i++) {
    std::vector<double> temp_weights;
    for (int j = 0; j < num_r; j++) {
      temp_weights.push_back(1.0);
    }
    Dim3_weight.push_back(temp_weights);
  }
  for (int i = 0; i < num_integal; i++) {
    Dim3_weight.at(i) = weights;
  }

  // 计算高斯积分
  auto Dx_weight = Dim3_weight.at(0);
  auto Dy_weight = Dim3_weight.at(1);
  auto Dz_weight = Dim3_weight.at(2);
  auto Dx_sample = Dim3_samples.at(0);
  auto Dy_sample = Dim3_samples.at(1);
  auto Dz_sample = Dim3_samples.at(2);
  // 一重积分
  if (num_integal == 1) {
    for (int i = 0; i < num_r; i++) {
      *result += Dx_sample.at(i) * TOOL::FuncCal(func, Dx_weight.at(i));
    }
  }
  // 二重积分
  if (num_integal == 2) {
    for (int i = 0; i < num_r; i++) {
      auto temp_ri = Dx_sample.at(i);
      auto temp_ai = Dx_weight.at(i);
      for (int j = 0; j < num_r; j++) {
        auto temp_rj = Dy_sample.at(j);
        auto temp_aj = Dx_weight.at(j);
        *result += temp_ai * temp_aj * TOOL::FuncCal(func, temp_ri, temp_rj);
      }
    }
  }
  // 三重积分
  if (num_integal == 3) {
    for (int i = 0; i < num_r; i++) {
      auto temp_ri = Dx_sample.at(i);
      auto temp_ai = Dx_weight.at(i);
      for (int j = 0; j < num_r; j++) {
        auto temp_rj = Dy_sample.at(j);
        auto temp_aj = Dy_weight.at(j);
        for (int k = 0; k < num_r; k++) {
          auto temp_rk = Dy_sample.at(k);
          auto temp_ak = Dz_weight.at(k);
          *result += temp_ai * temp_aj * temp_ak *
                     TOOL::FuncCal(func, temp_ri, temp_rj, temp_rk);
        }
      }
    }
  }

  return result;
}
} // namespace TOOL