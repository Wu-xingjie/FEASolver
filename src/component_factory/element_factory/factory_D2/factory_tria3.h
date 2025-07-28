/*
 * @Author: wxj wxj122350@126.com
 * @Date: 2025-07-28 15:32:16
 * @LastEditors: wxj wxj122350@126.com
 * @LastEditTime: 2025-07-28 15:36:44
 * @FilePath: /FEASolver/src/component_factory/element_factory/factory_D2/factory_tria3.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "component/element/D2/TRI3.h"
#include "component_factory/factory_base.h"

namespace FACTORY {
class TRIA3Factory : public FactoryBase {
public:
  TRIA3Factory() = default;
  ~TRIA3Factory() = default;
  virtual boost::shared_ptr<COMPONENT::CompBase> CreateComp() override;
};
} // namespace FACTORY