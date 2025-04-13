#pragma once

#include "coordinate_base.h"
#include "id_manager/id_mananger.h"
namespace COMPONENT {
class CartesianCoord : public CoordBase {
public:
  CartesianCoord() = default;
  ~CartesianCoord() = default;

  CartesianCoord(const CartesianCoord &p);

  int GetID() override;
  vec_3 getORIGIN() override;
  std::array<vec_3, 3> getVEC() override;

  void SetComp(const file_data &datas) override;
  comp_type Type() override;
};
} // namespace COMPONENT