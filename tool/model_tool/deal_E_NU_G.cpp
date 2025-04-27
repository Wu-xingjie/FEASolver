#include "deal_E_NU_G.h"
namespace TOOL {
    // 根据E,NU,G三者之间的关系通过任意两个已知数推出另外一个未知数
    std::array<double, 3> DealENuG(boost::optional<double> E,
                                   boost::optional<double> NU,
                                   boost::optional<double> G) {
      std::array<double, 3> result;
      try {
        if (E.is_initialized() && NU.is_initialized() && !G.is_initialized()) {
          G = E.get() / (2.0 * (1.0 + NU.get()));
          result[0] = E.get();
          result[1] = NU.get();
          result[2] = G.get();
        } else if (!E.is_initialized() && NU.is_initialized() &&
                   G.is_initialized()) {
          E = G.get() * (2.0 * (1.0 + NU.get()));
          result[0] = E.get();
          result[1] = NU.get();
          result[2] = G.get();
        } else if (E.is_initialized() && !NU.is_initialized() &&
                   G.is_initialized()) {
          NU = E.get() / (2 * G.get()) - 1.0;
          result[0] = E.get();
          result[1] = NU.get();
          result[2] = G.get();
        } else if (E.is_initialized() && NU.is_initialized() &&
                   G.is_initialized()) {
          result[0] = E.get();
          result[1] = NU.get();
          result[2] = G.get();
        } else {
          throw "E,NU,G中已知数个数最小为2";
        }
      } catch (const char* e) {
        std::cout << e << '\n';
      }
      return result;
    }
    }  // namespace TOOL