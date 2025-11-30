#include "static_solver.h"
#include "component/element/elemen_base.h"
#include "component/load/load_base.h"
#include "thread_pool/thread_pool.h"

namespace SOLVER {
void StaticSolver::AssembleMatrix() {
  // 生成单元刚度矩阵和载荷列阵
  TOOL::ThreadPool thread_pool(4, "StaticSolver::AssembleMatrix");
  for (auto comp_elem : _model._element) {
    auto generate_k = [comp_elem, this]() {
      auto base_elem =
          boost::dynamic_pointer_cast<COMPONENT::ElemBase>(comp_elem);
      base_elem->GenerateK(this->_model);
    };
    thread_pool.add_task(generate_k);
  }
  thread_pool.close();
  std::cout << "[INFO]:(StaticSolver::AssembleMatrix)>>>线程池关闭" << std::endl;

  // for (auto comp_elem : _model._element) {
  // auto base_elem =
  // boost::dynamic_pointer_cast<COMPONENT::ElemBase>(comp_elem);
  // base_elem->GenerateK(_model);
  // }

  for (auto comp_load : _model._load) {
    auto base_load =
        boost::dynamic_pointer_cast<COMPONENT::LoadBase>(comp_load);
    base_load->GenLoadVec(_model);
  }

  // 矩阵组装
  ASSEMBLE::MatrixAssemble matrix_assemble(_model);
  matrix_assemble.AssembleK();
  matrix_assemble.AssembleLoad();
  matrix_assemble.AddConstrain();

  // 去除额外自由度
  matrix_assemble.GetExtraDof();
  _K = matrix_assemble.RemoveExtraMatrixDof();
  _f = matrix_assemble.RemoveExtraLoadDof();
  _valid_dof = matrix_assemble.OutputValidDofSerial();
}

std::map<std::string, double> StaticSolver::solver() {
  auto values = _K.inverse() * _f;
  if (values.size() != _valid_dof.size()) {
    throw std::runtime_error(
        "[ERROR]:func(StaticSolver::solver)>>>结构静力求解器自由度采集错误！");
  }
  std::map<std::string, double> result;
  for (int i = 0; i < values.size(); i++) {
    result[_valid_dof.at(i)] = values(i);
  }
  return result;
}

} // namespace SOLVER