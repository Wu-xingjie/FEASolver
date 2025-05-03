#pragma once
#include "eigen3/Eigen/Dense"
#include "load_base.h"
#include <array>
namespace COMPONENT {
// 定义作用在节点上的静态集中载荷
class ConcentratedForce : public LoadBase {
public:
  ConcentratedForce() { _load_type = LoadBase::load_type::concentrated_force; }
  virtual ~ConcentratedForce() = default;
  // 获取载荷坐标系id
  int GetCoordId() { return _coord_id; }
  // 获取载荷大小
  double GetConstratedForce() { return _scale; };
  // 获取载荷方向
  std::array<double, 3> GetVec() {
    std::array<double, 3> result{_x, _y, _z};
    return result;
  }
  void SetComp(const file_data &datas) override;
  void GenLoadVec(const MODEL::Model &model) override;

private:
  int _nid;                  // 节点id
  int _coord_id;             // 载荷坐标系id
  double _scale;             // 力的大小
  double _x;                 // 载荷x方向
  double _y;                 // 载荷y方向
  double _z;                 // 载荷z方向
};
} // namespace COMPONENT