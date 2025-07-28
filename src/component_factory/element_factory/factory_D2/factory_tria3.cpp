/*
 * @Author: wxj wxj122350@126.com
 * @Date: 2025-07-28 15:32:16
 * @LastEditors: wxj wxj122350@126.com
 * @LastEditTime: 2025-07-28 15:37:13
 * @FilePath: /FEASolver/src/component_factory/element_factory/factory_D2/factory_tria3.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "factory_tria3.h"

#include "model_mapper/register.h"

namespace FACTORY {
boost::shared_ptr<COMPONENT::CompBase> TRIA3Factory::CreateComp() {
  return boost::make_shared<COMPONENT::Tri3>();
}
}  // namespace FACTORY