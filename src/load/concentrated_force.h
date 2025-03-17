#pragma once
#include "eigen3/Eigen/Dense"
#include "load_base.h"
namespace LOAD {
// 定义作用在节点上的静态集中载荷
class ConcentratedForce : public LoadBase {
 public:
  ConcentratedForce() : LoadBase() {}
  ~ConcentratedForce() = default;
  // 拷贝构造函数
  ConcentratedForce(const ConcentratedForce& p);
  // 获取对象id
  int GetID();
  // 获取载荷坐标系id
  int GetCoordId();
  // 获取载荷大小
  Eigen::Vector3d GetConstratedForce();
  // 设置节点id
  void SetNodeId(const int& id);
  // 设置载荷坐标系id
  void SetCoordId(const int& id);
  void SetConstratedForce(const double& mag_x, const double& mag_y,
                          const double& mag_z);

 private:
  int _nid;       // 节点id
  int _coord_id;  // 载荷坐标系id
  double _mag_x;  // 载荷在指定坐标系下的x分量
  double _mag_y;  // 载荷在指定坐标系下的y分量
  double _mag_z;  // 载荷在指定坐标系下的z分量
};
}  // namespace LOAD
