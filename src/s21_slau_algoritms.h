#ifndef S21_SLAU_ALGORITHMS_S21
#define S21_SLAU_ALGORITHMS_S21

#include <mutex>
#include <thread>

#include "s21_tensor.h"

namespace s21 {
class SlauAlgorithms {
 public:
  SlauAlgorithms() = default;
  ~SlauAlgorithms() = default;

  static Tensor<double> calculate(const Tensor<double>&, const Tensor<double>&);
  static Tensor<double> calculateThread(const Tensor<double>&,
                                        const Tensor<double>&);

 private:
  static bool check(const double, const double);
  static void searchAndSwap(Tensor<double>&, const size_t);
  static void trianAlg(Tensor<double>&);
  static void trianAlgThread(Tensor<double>&);
  static void threadFirst(const size_t, Tensor<double>&);
  static void threadSecond(size_t&, const size_t, std::mutex&, Tensor<double>&);
  static void onesSet(Tensor<double>&, const size_t);
  static Tensor<double> calculateResult(Tensor<double>&);
};
};  // namespace s21

#endif  // S21_SLAU_ALGORITHMS_S21