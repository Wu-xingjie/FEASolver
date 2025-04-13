#include "register.h"
#include "component_factory/material_factory/factory_mat1.h"
#include "component_factory/property_factory/factory_prod.h"
#include "component_factory/node_factory/factory_node.h"
#include "component_factory/element_factory/factory_D1/factory_rod.h"

// 材料注册
MAPPER::Regist<FACTORY::Mat1Factory> _Mat1FactoryREGIST("MAT1");
// 节点注册
MAPPER::Regist<FACTORY::NodeFactory> _NodeFactoryREGIST("GRID");
// 属性注册
MAPPER::Regist<FACTORY::ProdFactory> _NodeFactoryREGIST("PROD");
// 单元注册
MAPPER::Regist<FACTORY::RodFactory> _RodFactoryREGIST("ROD");