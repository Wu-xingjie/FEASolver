#include "concentrated_force.h"

namespace LOAD {
ConcentratedForce::ConcentratedForce(const ConcentratedForce& p) {
  _id = p._id;
  _nid = p._nid;
  _coord_id = p._coord_id;
  _mag_x = p._mag_x;
  _mag_y = p._mag_y;
  _mag_z = p._mag_z;
}
}  // namespace LOAD