#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {

// 节点编号如图：
//        8——————————7
//       /          /|
//      /          / |
//     /          /  |
//    5——————————6   |
//    |   4      |   /3
//    |          |  /
//    |          | /
//    |__________|/
//    1          2
// 8节点6面体单元
class Chexa8 : public ElemBase {
public:
  Chexa8();
  virtual ~Chexa8() = default;

  virtual void SetComp(const file_data &datas) override;
  virtual elem_type ElemType() override { return ElemBase::elem_type::chexa8; }
  virtual std::vector<int> GetNodes() override {
    return {_G1, _G2, _G3, _G4, _G5, _G6, _G7, _G8};
  }
  virtual void GenerateK(const MODEL::Model &model) override;
  virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) override;

private:
  double LenOfNode(const Eigen::Vector2d &n1, const Eigen::Vector2d &n2);

private:
  int _pid;
  int _G1;
  int _G2;
  int _G3;
  int _G4;
  int _G5;
  int _G6;
  int _G7;
  int _G8;
  Eigen::MatrixXd _loc_k;
};
} // namespace COMPONENT