#pragma once

#include "coordinate_base.h"
#include "id_manager/id_mananger.h"
namespace COMPONENT {
class CartesianCoord : public CoordBase {
public:
  CartesianCoord() = default;
  ~CartesianCoord() = default;

  CartesianCoord(const CartesianCoord &p);
  void SetComp(const file_data &datas) override;
  void TransToGeneralCoord() override;

private:
  int _N1;
  int _N2;
  int _N3;
};
} // namespace COMPONENT