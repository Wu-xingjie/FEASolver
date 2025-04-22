#include "coordinate_base.h"

namespace COMPONENT {
class GlobalCoord : public CoordBase {
 public:
  GlobalCoord() = default;
  virtual ~GlobalCoord() = default;

  virtual void SetComp(const file_data &datas) override;
  virtual void TransToGeneralCoord() override {
    std::cout << "[WARRING]>>>该坐标系为全局坐标,不需要进行转换" << std::endl;
  }
}
}  // namespace COMPONENT