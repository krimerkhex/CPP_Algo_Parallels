#include "s21_mul_algorithms.h"

namespace s21 {

Tensor<MatrixAlgorithms::value_type> MatrixAlgorithms::mulMatrix(
    const Tensor<value_type>& a, const Tensor<value_type>& b,
    const size_t count) {
  Tensor<value_type> result(a.rows(), b.colls());
  if (a.empty() || b.empty() || a.colls() != b.rows()) {
    std::cout << "Error mul matrix" << std::endl;
  } else {
    if (a.rows() == 1 && b.rows() == 1 && b.colls() == 1) {
      result[0] = a[0] * b[0];
    } else {
      if (count != 100) {
        conveirMul(result, a, b);
      } else {
        winogradMul(result, a, b, count);
      }
    }
  }
  return result;
}

void MatrixAlgorithms::conveirMul(Tensor<value_type>& a,
                                  const Tensor<value_type>& b,
                                  const Tensor<value_type>& c) {
  std::mutex mut;
  Tensor<value_type> rowFactor(a.rows()), collFactor(a.colls());
  Tensor<int> rowCheck(a.rows()), collCheck(a.colls());
  std::thread th1([&] {
    for (size_t j(0); j < (b.colls() / 2); ++j) {
      for (size_t i(0); i < b.rows(); ++i) {
        rowFactor[i] += b(i, j * 2) * b(i, (j * 2) + 1);
        rowCheck[i] = 1;
      }
    }
  });
  std::thread th2([&] {
    for (size_t i(0); i < (c.rows() / 2); ++i) {
      for (size_t j(0); j < c.colls(); ++j) {
        collFactor[j] += c(i * 2, j) * c((i * 2) + 1, j);
        collCheck[j] = 1;
      }
    }
  });
  std::thread th3([&] {
    for (size_t i(0); i < a.rows(); ++i) {
      for (size_t j(0); j < a.colls();) {
        if (rowCheck[i] && collCheck[j]) {
          a(i, j) = -rowFactor[i] - collFactor[j];
          for (size_t k(0); k < (b.colls() / 2); ++k) {
            a(i, j) += (b(i, k * 2) + c((k * 2) + 1, j)) *
                       (b(i, (k * 2) + 1) + c(k * 2, j));
          }
          ++j;
        } else {
          continue;
        }
      }
    }
  });
  th1.join();
  th2.join();
  th3.join();
  if (b.colls() % 2) {
    for (size_t i(0); i < a.rows(); ++i) {
      for (size_t j(0); j < a.colls(); ++j) {
        a(i, j) += b(i, -1) * c(-1, j);
      }
    }
  }
}

void MatrixAlgorithms::winogradMul(Tensor<value_type>& a,
                                   const Tensor<value_type>& b,
                                   const Tensor<value_type>& c,
                                   const size_t count) {
  std::mutex mut;
  Tensor<value_type> rowFactor(a.rows()), collFactor(a.colls());
  size_t midAxis(b.colls() / 2);
  size_t im(0), jm(0), i(0);
  forThread(
      [&] {
        while (jm < midAxis) {
          mut.lock();
          size_t jt(jm);
          ++jm;
          mut.unlock();
          if (jt < midAxis) {
            for (size_t i(0); i < b.rows(); ++i) {
              rowFactor[i] += b(i, jt * 2) * b(i, (jt * 2) + 1);
            }
          }
        }
      },
      !count ? midAxis : count);
  forThread(
      [&] {
        while (im < midAxis) {
          mut.lock();
          size_t it(im);
          ++im;
          mut.unlock();
          if (it < midAxis) {
            for (size_t j(0); j < c.colls(); ++j) {
              collFactor[j] += c(it * 2, j) * c((it * 2) + 1, j);
            }
          }
        }
      },
      !count ? midAxis : count);
  forThread(
      [&] {
        while (i < a.rows()) {
          mut.lock();
          size_t it(i);
          ++i;
          mut.unlock();
          if (it < a.rows()) {
            for (size_t j(0); j < a.colls(); ++j) {
              a(it, j) = -rowFactor[it] - collFactor[j];
              for (size_t k(0); k < midAxis; ++k) {
                a(it, j) += (b(it, k * 2) + c((k * 2) + 1, j)) *
                            (b(it, (k * 2) + 1) + c(k * 2, j));
              }
            }
          }
        }
      },
      !count ? a.rows() : count);
  if (b.colls() % 2) {
    for (size_t i(0); i < a.rows(); ++i) {
      for (size_t j(0); j < a.colls(); ++j) {
        a(i, j) += b(i, midAxis * 2) * c(midAxis * 2, j);
      }
    }
  }
}

void MatrixAlgorithms::forThread(const std::function<void(void)>& fn,
                                 const unsigned count) {
  std::vector<std::thread> th;
  for (unsigned i(0); i < count; ++i) {
    th.push_back(std::thread(fn));
  }
  for (auto& i : th) i.join();
}

};  // namespace s21