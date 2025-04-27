#include "length_node_to_node.h"
#include "component/coordinate/global_coordinate.h"
#include "geometry_tool/cos_of_vectors.h"
#include "model_tool/get_comp_by_id.h"
#include "trans_coord_to_coord.h"
namespace TOOL {
double LenNode2Node(const MODEL::Model &model,
                    const boost::shared_ptr<COMPONENT::Node> n1,
                    const boost::shared_ptr<COMPONENT::Node> n2) {
  double result = 0;
  COMPONENT::vec_3 loc_n1 = n1->get_location();
  COMPONENT::vec_3 loc_n2 = n2->get_location();
  // 两节点在同一坐标系下时
  if (n1->GetCoord() == n2->GetCoord()) {
    COMPONENT::vec_3 vec_n1_to_n2(loc_n2(0) - loc_n1(0), loc_n2(1) - loc_n1(1),
                                  loc_n2(2) - loc_n1(2));
    result = vec_n1_to_n2.norm();
  } else {
    // 两节点不在同一坐标系上时，先将两个节点变换到全局坐标系上，再计算节点间距离
    auto global_coord_base = model._coord.front();
    auto global_coord =
        boost::dynamic_pointer_cast<COMPONENT::CoordBase>(global_coord_base);
    if (!global_coord) {
      throw "[ERROR]>>func[LenNode2Node]:全局坐标系获取失败!";
    }
    // 把n1变换到全局坐标系下
    int cid_n1 = n1->GetCoord();
    auto cid_base_n1 =
        GetCompById(model, COMPONENT::CompBase::comp_type::coord, cid_n1);
    auto loc_coord_n1 =
        boost::dynamic_pointer_cast<COMPONENT::CoordBase>(cid_base_n1);
    if (!loc_coord_n1) {
      throw "[ERROR]>>func[LenNode2Node]:n1坐标系获取失败!";
    }
    Eigen::Matrix3d trans_n1_global =
        TransCoordToCoord(global_coord, loc_coord_n1);
    auto origin_n1 = loc_coord_n1->GetGeneralCoord()._coord_origin;
    auto globle_n1 = origin_n1 + trans_n1_global * loc_n1;
    //  把n2变换到全局坐标系下
    int cid_n2 = n2->GetCoord();
    auto cid_base_n2 =
        GetCompById(model, COMPONENT::CompBase::comp_type::coord, cid_n2);
    auto loc_coord_n2 =
        boost::dynamic_pointer_cast<COMPONENT::CoordBase>(cid_base_n2);
    if (!loc_coord_n2) {
      throw "[ERROR]>>func[LenNode2Node]:n2坐标系获取失败!";
    }
    Eigen::Matrix3d trans_n2_global =
        TransCoordToCoord(global_coord, loc_coord_n2);
    auto origin_n2 = loc_coord_n2->GetGeneralCoord()._coord_origin;
    auto globle_n2 = origin_n2 + trans_n2_global * loc_n2;
    // 在全局坐标系下求出两节点间的距离
    COMPONENT::vec_3 vec_n1_to_n2(globle_n1(0) - globle_n2(0),
                                  globle_n1(1) - globle_n2(1),
                                  globle_n1(2) - globle_n2(2));
    result = vec_n1_to_n2.norm();
  }
  return result;
}
} // namespace TOOL