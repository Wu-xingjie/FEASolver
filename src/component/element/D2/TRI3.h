/*
 * @Author: wxj wxj122350@126.com
 * @Date: 2025-07-06 10:52:50
 * @LastEditors: wxj wxj122350@126.com
 * @LastEditTime: 2025-07-10 21:37:28
 * @FilePath: /FEASolver/src/component/element/D2/TRI3.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "component/element/elemen_base.h"
#include "fea_model/fea_model.h"

namespace COMPONENT {
// class Tri3 : public ElemBase {
//  public:
//   Tri3();
//   virtual ~Tri3() = default;

//   virtual void SetComp(const file_data &datas) override;
//   virtual elem_type ElemType() override { return ElemBase::elem_type::tri3; }
//   virtual std::vector<int> GetNodes() override { return {_G1, _G2, _G3}; }
//   virtual void GenerateK(const MODEL::Model &model) override;
//   virtual Eigen::MatrixXd GetGlobalK(const MODEL::Model &model) override;

//  protected:
//   // 计算三节点三角形单元形函数（面积坐标）的偏导数
//   boost::shared_ptr<double> AreaCoordPartialDerivate(const vec_3 &n1,
//                                                      const vec_3 &n2,
//                                                      const char &lab);
//   void SetElemOfMatrixB(Eigen::MatrixXd &B, const int &r, const int &c, const char &p,
//                  const boost::shared_ptr<double> val);

//  private:
//   int _pid;
//   int _G1;
//   int _G2;
//   int _G3;
//   Eigen::MatrixXd _loc_k;
// };
}  // namespace COMPONENT