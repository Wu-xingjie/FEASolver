#include "component/coordinate/coordinate_base.h"
namespace TOOL {
Eigen::Matrix3d CoordToCoord(  
    const boost::shared_ptr<COMPONENT::CoordBase> base,
    const boost::shared_ptr<COMPONENT::CoordBase> obj);
}