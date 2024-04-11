#ifndef S21_MUL_ALGORITHMS_H
#define S21_MUL_ALGORITHMS_H

#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "s21_tensor.h"

namespace s21 {
class MatrixAlgorithms {
 public:
  using value_type = double;
  MatrixAlgorithms() = default;
  ~MatrixAlgorithms() = default;

  static Tensor<value_type> mulMatrix(const Tensor<value_type>&,
                                      const Tensor<value_type>&, const size_t);

 private:
  static void winogradMul(Tensor<value_type>&, const Tensor<value_type>&,
                          const Tensor<value_type>&, const size_t);
  static void conveirMul(Tensor<value_type>&, const Tensor<value_type>&,
                         const Tensor<value_type>&);
  static void forThread(const std::function<void(void)>&, const unsigned);
};
};  // namespace s21

#endif  // S21_MUL_ALGORITHMS_H