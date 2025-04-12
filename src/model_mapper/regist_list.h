#include "register.h"
#include "component_factory/material_factory/factory_mat1.h"
#include "component_factory/property_factory/factory_prod.h"
#include "component_factory/node_factory/factory_node.h"

MAPPER::Regist<FACTORY::Mat1Factory> _Mat1FactoryREGIST("MAT1");
MAPPER::Regist<FACTORY::NodeFactory> _NodeFactoryREGIST("GRID");
MAPPER::Regist<FACTORY::ProdFactory> _NodeFactoryREGIST("PROD");