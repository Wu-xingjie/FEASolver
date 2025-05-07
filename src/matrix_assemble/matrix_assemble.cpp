#include "matrix_assemble.h"
#include "component/element/elemen_base.h"

namespace ASSEMBLE {

MatrixAssemble::MatrixAssemble(const MODEL::Model &model) {
  _model = model;
  for (auto elem : _model._element) {
    auto base_elem = boost::dynamic_pointer_cast<COMPONENT::ElemBase>(elem);
    if (!base_elem) {
      throw "[ERROR]:func(MatrixAssemble)>>>有单元转换失败！";
    }
    _dof += base_elem->GetNodeNum();
  }
  _matrix_k = Eigen::MatrixXd::Zero(_dof, _dof);
  _vector_f = Eigen::VectorXd::Zero(_dof, 1);
}

void MatrixAssemble::AssembleK(const MODEL::Model &model) {
    
}

} // namespace ASSEMBLE